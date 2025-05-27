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
                task();
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

            if (data->dwRequestID == manager->liveGroup_.requestId) {
                manager->ParseGroupValues(data, manager->liveGroup_);
            } else if (data->dwRequestID == manager->feedbackGroup_.requestId) {
                manager->ParseGroupValues(data, manager->feedbackGroup_);
            }
            break;
        }

        // Handle other message types here

        default:
            break;
    }
}

void SimManager::AddLiveVariable(const std::string& name, const std::string& unit) {
    QueueTask([=, this] {
        if (liveGroup_.ContainsVariable(name)) {
            LogWarn("Variable already registered in group: " + name);
            return;
        }

        liveGroup_.variables.push_back({ name, unit });
        liveGroup_.Register(hSimConnect);

        HRESULT hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            liveGroup_.requestId,
            liveGroup_.definitionId,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            // SIMCONNECT_PERIOD_SECOND
            SIMCONNECT_PERIOD_VISUAL_FRAME
        );
        if (FAILED(hr)) {
            LogError("Failed to request LiveGroup data");
        }
    });
}

void SimManager::SubscribeToVariable(const std::string& name, VariableUpdateCallback callback) {
    std::unique_lock lock(callbackMutex_);
    updateCallbacks_[name].push_back(std::move(callback));
}

void SimManager::AddFeedbackVariable(const std::string& name, const std::string& unit) {
    QueueTask([=, this] {
        if (feedbackGroup_.ContainsVariable(name)) {
            LogWarn("Variable already registered in group: " + name);
            return;
        }

        feedbackGroup_.variables.push_back({ name, unit });
        feedbackGroup_.Register(hSimConnect);

        HRESULT hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            feedbackGroup_.requestId,
            feedbackGroup_.definitionId,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            SIMCONNECT_PERIOD_SECOND
        );
        if (FAILED(hr)) {
            LogError("Failed to request FeedbackGroup data");
        }
    });
}

void SimManager::RemoveLiveVariables() {
    QueueTask([=, this] {
        HRESULT hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            liveGroup_.requestId,
            liveGroup_.definitionId,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            SIMCONNECT_PERIOD_NEVER
        );
        if (FAILED(hr)) {
            LogError("Failed to request NEVER liveGroup data");
        }

        liveGroup_.variables.clear();
        liveGroup_.DeRegister(hSimConnect);
    });
}

void SimManager::RemoveFeedbackVariables() {
    QueueTask([=, this] {
        HRESULT hr = SimConnect_RequestDataOnSimObject(
            hSimConnect,
            feedbackGroup_.requestId,
            feedbackGroup_.definitionId,
            SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
            SIMCONNECT_PERIOD_NEVER
        );
        if (FAILED(hr)) {
            LogError("Failed to request NEVER FeedbackGroup data");
        }

        feedbackGroup_.variables.clear();
        feedbackGroup_.DeRegister(hSimConnect);
    });
}

void SimManager::ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const SimGroup& group) {
    const BYTE* raw = reinterpret_cast<const BYTE*>(&data->dwData);

    for (const auto& var : group.variables) {
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
            auto it = updateCallbacks_.find(var.name);
            if (it != updateCallbacks_.end()) {
                for (const auto& cb : it->second) {
                    cb(var.name, newVal);
                }
            }
        }
    }
}

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
