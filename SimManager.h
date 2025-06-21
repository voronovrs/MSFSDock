#pragma once

#include <Windows.h>
#include <SimConnect.h>
#include "Logger.h"
#include "SimVar.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


class SimManager {
public:
    static SimManager& Instance();  // Singleton accessor

    SimManager(const SimManager&) = delete;
    SimManager& operator=(const SimManager&) = delete;

    void Start();
    void Stop();

    void QueueTask(std::function<void()> task);

    std::optional<double> GetVariableValue(const std::string& name) const;
    std::string GetVariableAsString(const std::string& name, int precision = 0) const;

    using VariableUpdateCallback = std::function<void(const std::string& name, double value)>;
    std::unordered_map<std::string, std::vector<VariableUpdateCallback>> updateCallbacks_;
    std::shared_mutex callbackMutex_;
    void SubscribeToVariable(const std::string& name, VariableUpdateCallback callback);


    void RegisterSimVars(const std::vector<SimVarDefinition>& vars);
    void DeregisterSimVars(const std::vector<SimVarDefinition>& vars);
    void RegisterVariables();
    void DeregisterVariables();

private:
    SimManager();  // Private constructor
    ~SimManager();

    void Run();
    static void CALLBACK DispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

    HANDLE hSimConnect = nullptr;
    std::thread simThread;
    std::atomic<bool> running = false;

    std::queue<std::function<void()>> simTasks;
    std::mutex simTaskMutex;
    std::condition_variable simTaskCV;

    std::vector<SimVarDefinition> variables_;

    std::unordered_map<std::string, double> variableValues_;
    mutable std::shared_mutex variableMutex_;
    void ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const DEFINITIONS group);

    std::mutex executionMutex_;

    void AddNewVariables(const std::vector<SimVarDefinition>& incoming);
    void RmUnusedVariables(const std::vector<SimVarDefinition>& incoming);
};
