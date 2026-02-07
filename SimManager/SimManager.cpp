#include "SimManager.hpp"
#include "plugin/Logger.hpp"
#include "ui/UIManager.hpp"
#include <sstream>
#include <iomanip>


void SimManager::AddSimVars(std::vector<SimVarDefinition>& vars) {
    std::lock_guard lock(mutex_);
    for (auto& v : vars) {
        if (!v.IsValid())
            continue;
        auto it = vars_.find(v.name);
        if (it == vars_.end()) {
            v.used = 1;
            vars_.emplace(v.name, v);
            LogInfo("SimManager: Added new variable: " + v.name + " group: " + std::to_string(v.group));
        } else {
            it->second.IncrementUsage();
            LogInfo("SimManager: Increment usage for variable: " + v.name  + " usage: " + std::to_string(it->second.used));
        }
    }
}

void SimManager::RemoveSimVars(const std::vector<SimVarDefinition>& vars) {
    {
    std::lock_guard lock(mutex_);
    for (auto& v : vars) {
        auto it = vars_.find(v.name);
        if (it != vars_.end()) {
            if (it->second.IsInUse()) {
                it->second.DecrementUsage();
                LogInfo("SimManager: Decrement usage for variable: " + v.name  + " usage: " + std::to_string(it->second.used));
            }
            if (!it->second.IsInUse()) {
                continue;
            }
        } else {
            LogWarn("SimManager: Variable to remove not found: " + v.name);
        }
    }
    }
    DeregisterVariablesFromSim();
}

void SimManager::AddSimEvents(std::vector<SimEventDefinition>& events) {
    std::lock_guard lock(mutex_);
    for (auto& e : events) {
        auto it = events_.find(e.name);
        if (it == events_.end()) {
            e.used = 1;
            e.id = m_nextEventId++;
            events_.emplace(e.name, e);
            LogInfo("SimManager: Added new event: " + e.name + " id: " + std::to_string(e.id));
        } else {
            it->second.IncrementUsage();
            LogInfo("SimManager: Increment usage for event: " + e.name  + " usage: " + std::to_string(it->second.used));
        }
    }
}

void SimManager::RemoveSimEvents(const std::vector<SimEventDefinition>& events) {
    std::lock_guard lock(mutex_);
    for (auto& e : events) {
        auto it = events_.find(e.name);
        if (it != events_.end()) {
            if (it->second.IsInUse()) {
                it->second.DecrementUsage();
                LogInfo("SimManager: Decrement usage for event: " + e.name  + " usage: " + std::to_string(it->second.used));
            }
            if (!it->second.IsInUse()) {
                continue;
            }
        } else {
            LogWarn("SimManager: Event to remove not found: " + e.name);
        }
    }
}

void SimManager::ProcessPendingChanges() {
    if (!simReady) return;

    bool anyVarNeedsRegister = false;
    bool anyVarNeedsUnregister = false;
    bool anyEventNeedsRegister = false;
    bool anyEventNeedsUnregister = false;

    // Check if we need to register or deregister anything
    {
        std::lock_guard lock(mutex_);

        for (auto& [name, var] : vars_) {
            if (var.IsInUse() && !var.registered) {
                anyVarNeedsUnregister = true;
                anyVarNeedsRegister = true;
                break;
            }
        }

        for (auto& [name, var] : vars_) {
            // if (!var.IsInUse() && var.registered) {
            if (!var.IsInUse()) {
                anyVarNeedsUnregister = true;
                break;
            }
        }

        for (auto& [name, event] : events_) {
            if (event.IsInUse() && !event.registered) {
                anyEventNeedsRegister = true;
                break;
            }
        }

        for (auto& [name, event] : events_) {
            if (!event.IsInUse() && event.registered) {
                anyEventNeedsUnregister = true;
                break;
            }
        }
    }

    if (anyVarNeedsUnregister) {
        DeregisterVariablesFromSim();
    }
    if (anyVarNeedsRegister) {
        RegisterVariablesToSim();
    }
    if (anyEventNeedsUnregister) {
        DeregisterEventsFromSim();
    }
    if (anyEventNeedsRegister) {
        RegisterEventsToSim();
    }
}

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
    {
        std::lock_guard lock(simTaskMutex);
        simTasks.push(std::move(task));
    }
    simTaskCV.notify_one();
}

void SimManager::Run() {
    LogMessage("SimManager: run loop started, waiting for SimConnect.");

    while (running) {
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
                        LogError(std::string("SimManager: Task exception: ") + ex.what());
                    } catch (...) {
                        LogError("SimManager: Task unknown exception");
                    }
                }
                lock.lock();
            }
        }

        if (IsConnected()) {

            ProcessPendingChanges();

            HRESULT hr = SimConnect_CallDispatch(hSimConnect, DispatchProc, this);
            if (FAILED(hr)) {
                LogError("SimManager: SimConnect_CallDispatch failed -> safe disconnect");
                HandleSimDisconnect();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    HandleSimDisconnect();
    LogMessage("SimManager: run loop exited.");
}

// Try to connect to sim
void SimManager::EnsureConnected() {
    SimState expected = SimState::Disconnected;
    if (!simState.compare_exchange_strong(expected, SimState::Connecting)) {
        return;
    }

    simTasks.push([this]() {
        LogMessage("SimManager: trying to connect");

        HRESULT hr = SimConnect_Open(
            &hSimConnect,
            "MSFSDock Plugin",
            nullptr, 0, 0, 0);

        if (FAILED(hr)) {
            LogError("SimManager: SimConnect not available");
            hSimConnect = nullptr;
            simState = SimState::Disconnected;
            return;
        }

        simState = SimState::Connected;
        LogMessage("SimManager: SimConnect connected");
        OnSimConnected();
    });

    simTaskCV.notify_one();
}

bool SimManager::IsConnected() const {
    return simState == SimState::Connected
        && hSimConnect != nullptr
        && simReady;
}

// Update UI indicating sim is connected and ready
void SimManager::OnSimConnected() {
    simReady = true;
    UIManager::Instance().UpdateAll();
}

void SimManager::HandleSimDisconnect() {
    if (hSimConnect) {
        LogMessage("SimManager: closing SimConnect connection.");
        SimConnect_Close(hSimConnect);
        hSimConnect = nullptr;
    }

    simReady = false;
    simState = SimState::Disconnected;

    NotifyAndClearAllVariables();
    UIManager::Instance().UpdateAll();

    LogMessage("SimManager: SimConnect disconnected.");
}

void SimManager::NotifyAndClearAllVariables() {
    std::unique_lock lock(mutex_);

    // 1. Copy callbacks under callbackMutex_
    std::unordered_map<std::string, std::vector<CallbackEntry>> copy;
    {
        std::shared_lock cbLock(callbackMutex_);
        copy = updateCallbacks_;
    }

    // 2. Invoke callbacks without holding callbackMutex_
    for (auto& [name, entry] : vars_) {
        auto it = copy.find(name);
        if (it != copy.end()) {
            for (auto& cb : it->second) {
                try {
                    cb.cb(name, 0);
                } catch (...) {
                    LogError("callback exception");
                }
            }
        }
    }

    // 3. Mark all vars/events unregistered and reset vars values
    for (auto& [name, var] : vars_) {
        var.registered = false;
        var.value = 0.0;
    }

    for (auto& [name, evt] : events_)
        evt.registered = false;

    // 4. Clear registered groups
    registeredGroups_.clear();

    // 5. Clear order vectors
    liveVarOrder_.clear();
    feedbackVarOrder_.clear();

    // 6. Reset event ID
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

void SimManager::RegisterVariablesToSim() {
    QueueTask([this] {
        std::lock_guard lock(mutex_);

        if (!hSimConnect) {
            LogWarn("SimManager: RegisterVariablesToSim called but SimConnect is not ready.");
            return;
        }

        for (auto& [name, var] : vars_) {

            if (var.registered) {
                LogWarn("SimManager: Variable already registered to sim: " + var.name);
                continue;
            }

            HRESULT hr = SimConnect_AddToDataDefinition(
                hSimConnect,
                var.group,
                var.name.c_str(),
                var.GetUnit().c_str(),
                SIMCONNECT_DATATYPE_FLOAT64
            );

            if (FAILED(hr)) {
                LogError("SimManager: Failed to add data definition for variable: " + var.name);
            } else {
                LogInfo("SimManager: Added data definition for variable: " + var.name);
                var.registered = true;

                // Add to the corresponding group order vector
                if (var.group == LIVE_VARIABLE) {
                    liveVarOrder_.push_back(var.name);
                } else if (var.group == FEEDBACK_VARIABLE) {
                    feedbackVarOrder_.push_back(var.name);
                }
            }
        }

        for (int def = LIVE_VARIABLE; def <= FEEDBACK_VARIABLE; def++) {
            if (registeredGroups_.count(def) != 0) {
                LogWarn("SimManager: Group " + std::to_string(def) + " already registered.");
                continue;
            }
            HRESULT hr = SimConnect_RequestDataOnSimObject(
                hSimConnect,
                def,
                def,
                SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
                SIMCONNECT_PERIOD_VISUAL_FRAME
            );

            if (FAILED(hr)) {
                LogError("SimManager: Failed to request data for group: " + std::to_string(def));
            } else {
                LogInfo("SimManager: Added request data for group: " + std::to_string(def));
            }
            registeredGroups_.insert(def);
        }
    });
}

void SimManager::DeregisterVariablesFromSim() {
    QueueTask([this] {
        std::lock_guard lock(mutex_);

        if (!hSimConnect) {
            LogWarn("SimManager: DeregisterVariablesFromSim called but SimConnect is not ready.");
            return;
        }

        for (int def = LIVE_VARIABLE; def <= FEEDBACK_VARIABLE; def++) {
            if (registeredGroups_.count(def) == 0) {
                LogInfo("SimManager: Group " + std::to_string(def) + " not registered - skipping deregister.");
                continue;
            }

            // Stop updates from this group
            HRESULT hr = SimConnect_RequestDataOnSimObject(
                hSimConnect,
                def,
                def,
                SIMCONNECT_OBJECT_ID_USER_AIRCRAFT,
                SIMCONNECT_PERIOD_NEVER
            );

            if (FAILED(hr)) {
                LogError("SimManager: Failed to stop request for group: " + std::to_string(def));
            } else {
                LogInfo("SimManager: Stopped request for group: " + std::to_string(def));
            }

            // Clear definitions
            hr = SimConnect_ClearDataDefinition(hSimConnect, def);
            if (FAILED(hr)) {
                LogError("SimManager: Failed to clear data definition for group: " + std::to_string(def));
            } else {
                LogInfo("SimManager: Cleared data definition for group: " + std::to_string(def));
            }
            registeredGroups_.erase(def);
        }

        // Now clear our internal state
        for (auto it = vars_.begin(); it != vars_.end();) {
            if (!it->second.IsInUse()) {
                LogInfo("SimManager: Removing unused variable: " + it->second.name);
                it = vars_.erase(it);
                continue;
            }

            it->second.registered = false;
            ++it;
        }

        // Clear order vectors because ClearDataDefinition removes everything
        liveVarOrder_.clear();
        feedbackVarOrder_.clear();
    });
}

void SimManager::RegisterEventsToSim() {
    QueueTask([this] {
        for (auto& [name, event] : events_) {
            if (event.registered) {
                LogInfo("SimManager: Event: " + name + " already registered to sim with ID: " + std::to_string(event.id));
                continue;
            }

            HRESULT hr = SimConnect_MapClientEventToSimEvent(hSimConnect, event.id, event.name.c_str());
            if (FAILED(hr)) {
                LogError("SimManager: Failed to register event: " + name + " with id: " + std::to_string(event.id));
            } else {
                LogInfo("SimManager: Event registered: " + name + " with id: " + std::to_string(event.id));
                event.registered = true;
            }
        }
    });
}

void SimManager::DeregisterEventsFromSim() {
    // We can't remove event after registration, so just log it and keep info in case of future use
    // LogInfo("SimManager: Event not used: " + it->first);
}

bool SimManager::TryGetCachedValue(const std::string& name, double& outValue) {
    std::lock_guard lock(mutex_);

    auto it = vars_.find(name);
    if (it == vars_.end()) {
        return false;
    }

    outValue = it->second.value;
    return true;
}

SubscriptionId SimManager::SubscribeToVariable(const std::string& name, VariableUpdateCallback callback) {
    auto id = nextSubscriptionId_.fetch_add(1, std::memory_order_relaxed);
    {
        std::unique_lock lock(callbackMutex_);
        updateCallbacks_[name].push_back({id, std::move(callback)});
    }

    // Check if value already exist and calling callback.
    double currentValue = 0.0;
    bool haveValue = TryGetCachedValue(name, currentValue);

    if (haveValue) {
        std::shared_lock lock(callbackMutex_);
        auto it = updateCallbacks_.find(name);
        if (it != updateCallbacks_.end() && !it->second.empty() && it->second.back().id == id) {
            try {
                it->second.back().cb(name, currentValue);
            } catch(...) {
                LogError("SubscribeToVariable: Callback exception");
            }
        }
    }
    return id;
}

void SimManager::UnsubscribeFromVariable(const std::string& name, SubscriptionId id) {
    std::unique_lock lock(callbackMutex_);
    auto it = updateCallbacks_.find(name);
    if (it == updateCallbacks_.end())
        return;
    auto &vec = it->second;
    vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const CallbackEntry &e){ return e.id == id; }), vec.end());
    if (vec.empty())
        updateCallbacks_.erase(it);
}

void SimManager::ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const DEFINITIONS group) {
    const BYTE* raw = reinterpret_cast<const BYTE*>(&data->dwData);

    std::vector<std::pair<std::string, double>> updatedVars;

    {
        std::unique_lock lock(mutex_);

        // Select the correct order vector based on the group
        const std::vector<std::string>& orderVec =
            (group == LIVE_VARIABLE) ? liveVarOrder_ : feedbackVarOrder_;

        // Iterate in the exact order in which variables were registered in SimConnect
        // This is CRITICAL because SimConnect sends data in registration order
        for (const std::string& varName : orderVec) {
            auto it = vars_.find(varName);
            if (it == vars_.end()) {
                // Variable was removed from map but still exists in order vector
                // Advance pointer to maintain synchronization
                LogWarn("SimManager: Variable in order vector but not in map: " + varName);
                raw += sizeof(double);
                continue;
            }

            SimVarDefinition& var = it->second;

            // Read value even if not in use, to keep pointer synchronized
            double newVal = *reinterpret_cast<const double*>(raw);
            raw += sizeof(double);

            if (!var.registered || !var.IsInUse()) {
                // Variable is no longer in use, ignore value but we already advanced the pointer
                LogWarn("SimManager: Ignoring stale SimConnect value for variable: " + varName);
                continue;
            }

            if (!IsValueValid(newVal)) {
                LogWarn("SimManager: Rejected invalid value for variable: " + var.name + " = " + std::to_string(newVal));
                continue;
            }

            // Round it up
            double roundedVal = std::round(newVal * 1000.0) / 1000.0;

            if (var.value != roundedVal) {
                // LogInfo("SimManager: Got new value for variable: " + var.name + " = " + std::to_string(roundedVal));
                var.value = roundedVal;
                updatedVars.emplace_back(varName, roundedVal);
            }
        }
    }

    // Calling callbacks
    for (auto& [name, val] : updatedVars) {
        std::shared_lock lock(callbackMutex_);
        auto it = updateCallbacks_.find(name);
        if (it != updateCallbacks_.end()) {
            for (auto& entry : it->second) {
                try {
                    entry.cb(name, val);
                } catch (...) {
                    LogError("ParseGroupValues: Callback exception");
                }
            }
        }
    }

}

void SimManager::SendEvent(const std::string& name) {
    DWORD eventId = 0;

    {
        std::lock_guard lock(mutex_);
        auto it = events_.find(name);
        if (it == events_.end()) {
            LogError("SendEvent: event not found in events_: " + name);
            return;
        }

        if (!it->second.IsInUse()) {
            LogWarn("SendEvent: event exists but not in use: " + name);
            return;
        }

        if (!it->second.registered) {
            LogError("SendEvent: event not registered in sim: " + name);
            return;
        }

        eventId = it->second.id;
    }

    HRESULT hr = SimConnect_TransmitClientEvent(
        hSimConnect,
        SIMCONNECT_OBJECT_ID_USER,
        eventId,
        0,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST,
        SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY
    );

    if (FAILED(hr)) {
        LogError("SendEvent: failed to transmit event: " + name +
                 " id=" + std::to_string(eventId));
    } else {
        LogInfo("SendEvent: sent event " + name + " id=" + std::to_string(eventId));
    }
}
