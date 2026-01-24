#pragma once

#include <Windows.h>
#include <SimConnect.h>
#include "core/SimVar.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

using SubscriptionId = uint64_t;

enum class SimState {
    Disconnected,
    Connecting,
    Connected
};

class SimManager {
public:
    static SimManager& Instance();  // Singleton accessor

    SimManager(const SimManager&) = delete;
    SimManager& operator=(const SimManager&) = delete;

    void Start();
    void Stop();

    void AddSimVars(std::vector<SimVarDefinition>& vars);
    void RemoveSimVars(const std::vector<SimVarDefinition>& vars);

    void AddSimEvents(std::vector<SimEventDefinition>& events);
    void RemoveSimEvents(const std::vector<SimEventDefinition>& events);

    using VariableUpdateCallback = std::function<void(const std::string& name, double value)>;
    struct CallbackEntry { SubscriptionId id; VariableUpdateCallback cb; };
    SubscriptionId SubscribeToVariable(const std::string& name, VariableUpdateCallback callback);
    void UnsubscribeFromVariable(const std::string& name, SubscriptionId id);

    void SendEvent(const std::string& name);

    void EnsureConnected();
    bool IsConnected() const;

private:
    SimManager();  // Private constructor
    ~SimManager();

    std::atomic<SimState> simState = SimState::Disconnected;
    std::unordered_map<std::string, std::vector<CallbackEntry>> updateCallbacks_;
    std::shared_mutex callbackMutex_;
    std::atomic<SubscriptionId> nextSubscriptionId_{1};

    HANDLE hSimConnect = nullptr;
    std::thread simThread;
    std::atomic<bool> running = false;
    std::atomic<bool> simReady = false;

    std::queue<std::function<void()>> simTasks;
    std::mutex executionMutex_;
    std::mutex mutex_;
    std::mutex simTaskMutex;
    std::condition_variable simTaskCV;

    DWORD m_nextEventId = 1000;

    std::map<std::string, SimVarDefinition> vars_;
    std::map<std::string, SimEventDefinition> events_;
    std::unordered_set<int> registeredGroups_;

    void Run();
    static void CALLBACK DispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);
    void QueueTask(std::function<void()> task);
    bool TryGetCachedValue(const std::string& name, double& outValue);
    void HandleSimDisconnect(); // cleanup after disconnect
    void OnSimConnected();
    void NotifyAndClearAllVariables();

    void RegisterVariablesToSim();
    void DeregisterVariablesFromSim();
    void RegisterEventsToSim();
    void DeregisterEventsFromSim();
    void ProcessPendingChanges();

    void ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const DEFINITIONS group);

    static inline bool IsValueValid(double v) {
        if (std::isnan(v)) return false;
        if (std::isinf(v)) return false;
        if (v < -1e6 || v > 1e6) return false;
        return true;
    }
};
