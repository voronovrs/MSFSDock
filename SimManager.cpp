#include "SimManager.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>


SimManager& SimManager::Instance() {
    static SimManager instance;
    return instance;
}

SimManager::SimManager(/* args */) {}

SimManager::~SimManager() {
    Stop();
}

void SimManager::Start() {
    if (running) {
        return;
    }
    running = true;
    simThread = std::thread([this]{ Run(); });
}

void SimManager::Stop() {
    if (!running) {
        return;
    }
    running = false;
    simTaskCV.notify_one();
    if (simThread.joinable()) {
        simThread.join();
    }
}

void SimManager::QueueTask(std::function<void()> task) {
    if (!simReady) {
        std::lock_guard lock(pendingMutex_);
        pendingTasks_.push(std::move(task));
        return;
    }

    {
        std::lock_guard lock(simTaskMutex);
        simTasks.push(std::move(task));
    }
    simTaskCV.notify_one();
}

void SimManager::Run() {
    LogInfo("SimManager: run loop started, waiting for SimConnect.");

    while (running) {
        if (!hSimConnect) {
            HRESULT hr = SimConnect_Open(&hSimConnect, "MSFSDock Plugin", nullptr, 0, 0, 0);
            if (FAILED(hr)) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }
            LogInfo("SimManager: SimConnect connected.");
            OnSimConnected();
        }

        if (!simReady) {
            HRESULT checkHr = SimConnect_RequestDataOnSimObject(
                hSimConnect,
                LIVE_VARIABLE,
                LIVE_VARIABLE,
                SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
                SIMCONNECT_PERIOD_NEVER
            );
            if (SUCCEEDED(checkHr)) {
                LogInfo("SimManager: simReady = true (sim already running).");
                if (!simReady) {
                    OnSimStart();
                }
            }
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
                    try {
                        task();
                    } catch (const std::exception& ex) {
                        LogError(std::string("Task exception: ") + ex.what());
                    } catch (...) {
                        LogError("Task unknown exception");
                    }
                }
                lock.lock();
            }
        }

        if (hSimConnect) {
            HRESULT hr = SimConnect_CallDispatch(hSimConnect, DispatchProc, this);
            if (FAILED(hr)) {
                LogWarn("SimManager: SimConnect_CallDispatch failed -> safe disconnect");
                SafeDisconnect();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    SafeDisconnect();
    LogInfo("SimManager: run loop exited.");
}

void SimManager::OnSimConnected() {
    constexpr DWORD EVENT_FLIGHT_LOADED = 1;
    constexpr DWORD EVENT_SIM_START = 2;

    HRESULT hr;
    hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_FLIGHT_LOADED, "FlightLoaded");
    if (FAILED(hr)) {
        LogWarn("SimManager: failed to subscribe FlightLoaded event");
    }
    hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
    if (FAILED(hr)) {
        LogWarn("SimManager: failed to subscribe SimStart event");
    }

    // clear simReady until we receive the event
    simReady = false;
}

void SimManager::OnSimStart() {
    if (simReady) return;

    LogInfo("ONSIM START");
    simReady = true;

    // Run pending tasks
    {
        std::lock_guard lock(pendingMutex_);
        std::lock_guard lock2(simTaskMutex);

        while (!pendingTasks_.empty()) {
            simTasks.push(std::move(pendingTasks_.front()));
            pendingTasks_.pop();
        }
    }

    simTaskCV.notify_one();

    DeregisterVariables();
    RegisterVariables();
    RegisterEventsInSim();

    LogInfo("All pending tasks flushed to main SimConnect queue");
}

void SimManager::SafeDisconnect() {
    if (hSimConnect) {
        LogInfo("SimManager: closing SimConnect connection.");
        SimConnect_Close(hSimConnect);
        hSimConnect = nullptr;
    }

    simReady = false;

    NotifyAndClearAllVariables();

    LogInfo("SimManager: disconnected, will attempt reconnect.");
}

void SimManager::HandleSimDisconnect() {
    SafeDisconnect();
}

void SimManager::NotifyAndClearAllVariables() {
    std::unordered_map<std::string, std::vector<VariableUpdateCallback>> callbacksCopy;

    // copy callbacks
    {
        std::shared_lock lock(callbackMutex_);
        callbacksCopy = updateCallbacks_;
    }

    for (auto& [name, cbs] : callbacksCopy) {
        for (auto& cb : cbs) {
            cb(name, 0);
        }
    }

    // Clear variables
    {
        std::unique_lock lock(variableMutex_);
        variableValues_.clear();
    }

    // Reset event ID
    m_nextEventId = 1000;
}

void CALLBACK SimManager::DispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
    // Handle received SimConnect messages
    auto* manager = static_cast<SimManager*>(pContext);
    if (!manager) {
        return;
    }
    // LogInfo("Recieved dwID " + std::to_string(pData->dwID));
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_QUIT:
            LogWarn("SimManager: MSFS closed or disconnected (QUIT). Scheduling disconnect handling.");

            // Signal your thread to exit or try reconnect
            manager->QueueTask([manager] {
                manager->HandleSimDisconnect();
            });
            break;
        case SIMCONNECT_RECV_ID_EXCEPTION:
        {
            auto* ex = reinterpret_cast<SIMCONNECT_RECV_EXCEPTION*>(pData);
            LogError("SimConnect exception received: " + std::to_string(ex->dwException) + " " + std::to_string(ex->dwIndex));
            break;
        }
        case SIMCONNECT_RECV_ID_EVENT:
        {
            auto* evt = reinterpret_cast<SIMCONNECT_RECV_EVENT*>(pData);
            LogInfo(std::string("SimManager: received event id ") + std::to_string(evt->uEventID));
            constexpr DWORD EVENT_FLIGHT_LOADED = 1;
            constexpr DWORD EVENT_SIM_START = 2;

            if (evt->uEventID == EVENT_SIM_START) {
                LogInfo("SIM READY: EVENT_SIM_START received");
                if (!manager->simReady) {
                    manager->OnSimStart();
                }
            }
            break;
        }
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto* data = reinterpret_cast<const SIMCONNECT_RECV_SIMOBJECT_DATA*>(pData);

            if (data->dwRequestID == LIVE_VARIABLE) {
                manager->ParseGroupValues(data, LIVE_VARIABLE);
            } else if (data->dwRequestID == FEEDBACK_VARIABLE) {
                manager->ParseGroupValues(data, FEEDBACK_VARIABLE);
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
            LogInfo("Adding new variable: " + def.name + " to group: " + std::to_string(def.group));
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

void SimManager::AddNewEvents(const std::vector<std::string>& incoming) {
    for (const auto& name : incoming) {
        auto it = std::find_if(
            registeredEvents_.begin(), registeredEvents_.end(),
            [&name](const EventDefinition& e) { return e.name == name; }
        );

        if (it == registeredEvents_.end()) {
            DWORD id = m_nextEventId++;
            registeredEvents_.push_back({name, id});
        }
    }
}

void SimManager::RegisterEventsInSim() {
    QueueTask([this] {
        for (const auto& evt : registeredEvents_) {
            HRESULT hr = SimConnect_MapClientEventToSimEvent(hSimConnect, evt.id, evt.name.c_str());
            if (FAILED(hr)) {
                LogError("Failed to register event: " + evt.name + " id: " + std::to_string(evt.id));
            } else {
                LogInfo("Event registered: " + evt.name + " id: " + std::to_string(evt.id));
            }
        }
    });
}

void SimManager::RegisterEvents(const std::vector<std::string>& events) {
    AddNewEvents(events);
    RegisterEventsInSim();
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
                    {
                        std::unique_lock lock(variableMutex_);
                        variableValues_.erase(reg.name);
                    }
                    LogInfo("Remove unused variable: " + reg.name + " from group: " + std::to_string(reg.group));
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
            HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect,
                                                        var.group,
                                                        var.name.c_str(),
                                                        var.GetUnit().c_str(),
                                                        SIMCONNECT_DATATYPE_FLOAT64);
            if (FAILED(hr)) {
                LogError("Failed to add data definition for variable: " + var.name);
            } else {
                LogInfo("Added data definition for variable: " + var.name);
            }
        }

        for (int def = LIVE_VARIABLE; def <= FEEDBACK_VARIABLE; def++) {
            HRESULT hr = SimConnect_RequestDataOnSimObject(
                hSimConnect,
                def,
                def,
                SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
                SIMCONNECT_PERIOD_VISUAL_FRAME
            );

            if (FAILED(hr)) {
                LogError("Failed to request data for group: " + std::to_string(def));
            } else {
                LogInfo("Added request data for group: " + std::to_string(def));
            }
        }
    });
}

bool SimManager::TryGetCachedValue(const std::string& name, double& outValue) {
    std::shared_lock lock(variableMutex_);
    auto it = variableValues_.find(name);
    if (it == variableValues_.end())
        return false;

    outValue = it->second;
    return true;
}

void SimManager::DeregisterVariables() {
    QueueTask([=, this] {
        for (int def = LIVE_VARIABLE; def <= FEEDBACK_VARIABLE; def++) {
            HRESULT hr = SimConnect_RequestDataOnSimObject(
                hSimConnect,
                def,
                def,
                SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
                SIMCONNECT_PERIOD_NEVER
            );

            if (FAILED(hr)) {
                LogError("Failed to change request period to NEVER for group: " + std::to_string(def));
            } else {
                LogInfo("Changed request period to NEVER for group: " + std::to_string(def));
            }

            hr = SimConnect_ClearDataDefinition(hSimConnect, def);
            if (FAILED(hr)) {
                LogError("Failed to clear data definition for group: " + std::to_string(def));
            } else {
                LogInfo("Cleared data definition for group: " + std::to_string(def));
            }
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

        if (!IsValueValid(newVal)) {
            LogWarn("Rejected invalid value for variable: " + var.name + " = " + std::to_string(newVal));
            continue;
        }

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
            // LogInfo("Variable: " + var.name + " new value = " + std::to_string(newVal));
            auto it = updateCallbacks_.find(var.name);
            if (it != updateCallbacks_.end()) {
                for (const auto& cb : it->second) {
                    cb(var.name, newVal);
                }
            }
        }
    }
}

void SimManager::SendEvent(const std::string& name) {
    auto it = std::find_if(registeredEvents_.begin(), registeredEvents_.end(),
                           [&name](const EventDefinition& e) { return e.name == name; });
    if (it == registeredEvents_.end()) {
        LogError("SendEvent: event not registered: " + name);
        return;
    }

    DWORD id = it->id;

    HRESULT hr = SimConnect_TransmitClientEvent(
        hSimConnect,
        SIMCONNECT_OBJECT_ID_USER,
        id,
        0,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY
    );

    if (FAILED(hr)) {
        LogError("SendEvent: failed to transmit event: " + name + " id: " + std::to_string(id));
    }
}
