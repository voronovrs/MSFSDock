#include "SimManager.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>


SimManager& SimManager::Instance() {
    static SimManager instance;
    return instance;
}

SimManager::SimManager(/* args */) {}

SimManager::~SimManager() {}

void SimManager::Start() {
    running = true;
    simThread = std::thread(&SimManager::Run, this);
}

void SimManager::Stop() {
    running = false;
    simTaskCV.notify_one();
    if (simThread.joinable())
        simThread.join();
}

void SimManager::Run() {
    LogInfo("Waiting for SimConnect to start");

    while (running) {
        if (!hSimConnect) {
            HRESULT hr = SimConnect_Open(&hSimConnect, "MSFSDock Plugin", nullptr, 0, 0, 0);
            if (FAILED(hr)) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }
            LogInfo("SimConnect connected");
        }

        {
            std::unique_lock lock(simTaskMutex);
            simTaskCV.wait_for(lock, std::chrono::milliseconds(10));
            while (!simTasks.empty()) {
                auto task = std::move(simTasks.front());
                simTasks.pop();
                lock.unlock();
                {
                    std::unique_lock execLock(executionMutex_);
                    task();  // 🔒 Only one task runs at a time here
                }
                lock.lock();
            }
        }

        if (hSimConnect) {
            HRESULT hr = SimConnect_CallDispatch(hSimConnect, DispatchProc, this);
            if (FAILED(hr)) {
                LogError("SimConnect_CallDispatch failed. Disconnecting.");
                SimConnect_Close(hSimConnect);
                hSimConnect = nullptr;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (hSimConnect) {
        SimConnect_Close(hSimConnect);
        hSimConnect = nullptr;
    }
}

void SimManager::QueueTask(std::function<void()> task) {
    {
        std::lock_guard lock(simTaskMutex);
        simTasks.push(std::move(task));
    }
    simTaskCV.notify_one();
}

void CALLBACK SimManager::DispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
    // Handle received SimConnect messages
    auto* manager = static_cast<SimManager*>(pContext);
    // LogInfo("Recieved dwID " + std::to_string(pData->dwID));
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_QUIT:
            LogWarn("MSFS has closed or disconnected.");
            // Signal your thread to exit or try reconnect
            manager->Stop();
            break;

        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto* data = reinterpret_cast<const SIMCONNECT_RECV_SIMOBJECT_DATA*>(pData);

            if (data->dwRequestID == LIVE_VARIABLE) {
                manager->ParseGroupValues(data, LIVE_VARIABLE);
            // } else if (data->dwRequestID == manager->feedbackGroup_.requestId) {
                // manager->ParseGroupValues(data, manager->feedbackGroup_);
            }
            break;
        }

        // Handle other message types here

        default:
            break;
    }
}

void SimManager::AddNewVariables(const std::vector<SimVarDefinition>& incoming) {
    for (const auto& def : incoming) {
        auto it = std::find_if(
            variables_.begin(), variables_.end(),
            [&def](const SimVarDefinition& existing) {
                return existing.name == def.name;
            });

        if (it == variables_.end()) {
            LogInfo("Adding new variable " + def.name + " to group " + std::to_string(def.group));
            variables_.push_back(def);
        }
        for (auto& reg : variables_) {
            if (reg.name == def.name) {
                reg.IncrementUsage();
                // LogInfo("Variable " + reg.name + " usage+ " + std::to_string(reg.used));
                break;
            }
        }
    }
}

void SimManager::RegisterSimVars(const std::vector<SimVarDefinition>& vars) {
    QueueTask([=, this] {
        AddNewVariables(vars);
    });
}

void SimManager::RmUnusedVariables(const std::vector<SimVarDefinition>& incoming) {
    for (auto& reg : variables_) {
        auto it = std::find_if(
            incoming.begin(), incoming.end(),
            [&reg](const SimVarDefinition& existing) {
                return existing.name == reg.name;
            });
            if (it != incoming.end()) {
                if (reg.IsInUse()) {
                    reg.DecrementUsage();
                    // LogInfo("Variable " + reg.name + " usage- " + std::to_string(reg.used));
                }
                if (!reg.IsInUse()) {
                    LogInfo("Remove unused variable " + reg.name + " from group " + std::to_string(reg.group));
                }
            }
    }
    variables_.erase(
    std::remove_if(variables_.begin(), variables_.end(),
                   [](const SimVarDefinition& reg) { return !reg.IsInUse(); }),
                    variables_.end());
}

void SimManager::DeregisterSimVars(const std::vector<SimVarDefinition>& vars) {
    QueueTask([=, this] {
        RmUnusedVariables(vars);
    });
}

void SimManager::RegisterVariables() {
    QueueTask([=, this] {
        for (const auto& var : variables_) {
            HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, var.group, var.name.c_str(), var.GetUnit().c_str(), SIMCONNECT_DATATYPE_FLOAT64);
            if (FAILED(hr)) {
                LogError("Failed to register sim var: " + var.name);
            } else {
                LogInfo("Registered sim var: " + var.name);
            }
        }

        HRESULT hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            LIVE_VARIABLE,
            LIVE_VARIABLE,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            SIMCONNECT_PERIOD_VISUAL_FRAME
        );
        if (FAILED(hr)) {
            LogError("Failed to request LIVE_VARIABLE data");
        }

    });
}

void SimManager::DeregisterVariables() {
    QueueTask([=, this] {
        HRESULT hr;

        hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            LIVE_VARIABLE,
            LIVE_VARIABLE,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            SIMCONNECT_PERIOD_NEVER
        );
        if (FAILED(hr)) {
            LogError("Failed to request NEVER liveGroup data");
        }

        hr = SimConnect_ClearDataDefinition(hSimConnect, LIVE_VARIABLE);
        if (FAILED(hr)) {
            LogError("Failed to deregister sim vars definition " + std::to_string(LIVE_VARIABLE));
        } else {
            LogError("Deregister sim vars definition " + std::to_string(LIVE_VARIABLE));
        }
        hr = SimConnect_ClearDataDefinition(hSimConnect, FEEDBACK_VARIABLE);
        if (FAILED(hr)) {
            LogError("Failed to deregister sim vars definition " + std::to_string(FEEDBACK_VARIABLE));
        } else {
            LogError("Deregister sim vars definition " + std::to_string(FEEDBACK_VARIABLE));
        }
    });
}

void SimManager::SubscribeToVariable(const std::string& name, VariableUpdateCallback callback) {
    std::unique_lock lock(callbackMutex_);
    updateCallbacks_[name].push_back(std::move(callback));
}

void SimManager::ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const DEFINITIONS group) {
    const BYTE* raw = reinterpret_cast<const BYTE*>(&data->dwData);

    for (const auto& var : variables_) {
        if (var.group != group) {
            continue;
        }
        double newVal = *reinterpret_cast<const double*>(raw);
        raw += sizeof(double);

        bool changed = false;
        {
            std::unique_lock lock(variableMutex_);
            auto& old = variableValues_[var.name];
            if (old != newVal) {
                old = newVal;
                changed = true;
            }
        }

        if (changed) {
            std::shared_lock lock(callbackMutex_);
            LogInfo("Changed var " + var.name + " value " + std::to_string(newVal));
            auto it = updateCallbacks_.find(var.name);
            if (it != updateCallbacks_.end()) {
                for (const auto& cb : it->second) {
                    cb(var.name, newVal);
                }
            }
        }
    }
}

// void SimManager::ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const SimGroup& group) {
//     const BYTE* raw = reinterpret_cast<const BYTE*>(&data->dwData);

//     for (const auto& var : group.variables) {
//         double newVal = *reinterpret_cast<const double*>(raw);
//         raw += sizeof(double);

//         bool changed = false;
//         {
//             std::unique_lock lock(variableMutex_);
//             auto& old = variableValues_[var.name];
//             if (old != newVal) {
//                 old = newVal;
//                 changed = true;
//             }
//         }

//         if (changed) {
//             std::shared_lock lock(callbackMutex_);
//             auto it = updateCallbacks_.find(var.name);
//             if (it != updateCallbacks_.end()) {
//                 for (const auto& cb : it->second) {
//                     cb(var.name, newVal);
//                 }
//             }
//         }
//     }
// }

std::optional<double> SimManager::GetVariableValue(const std::string& name) const {
    std::shared_lock lock(variableMutex_);
    auto it = variableValues_.find(name);
    if (it != variableValues_.end())
        return it->second;
    return std::nullopt;
}

std::string SimManager::GetVariableAsString(const std::string& name, int precision) const {
    if (auto val = GetVariableValue(name)) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << *val;
        return oss.str();
    }
    return "-----";
}
