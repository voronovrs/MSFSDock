#pragma once

#include <Windows.h>
#include <SimConnect.h>
#include "Logger.h"
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
// #include "SimGroup.h"

struct SimVarDefinition {
    std::string name;
    std::string unit;
};

struct SimGroup {
    uint32_t definitionId;
    uint32_t requestId;
    std::vector<SimVarDefinition> variables;

    void Register(HANDLE hSimConnect) const {
        for (const auto& var : variables) {
            HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, definitionId, var.name.c_str(), var.unit.c_str(), SIMCONNECT_DATATYPE_FLOAT64);
            if (FAILED(hr)) {
                LogError("Failed to register sim var: " + var.name);
            } else {
                LogInfo("Registered sim var: " + var.name);
            }
        }
    }

    void DeRegister(HANDLE hSimConnect) const {
        LogInfo("Try to deregister sim vars with ID " + std::to_string(definitionId));
        HRESULT hr = SimConnect_ClearDataDefinition(hSimConnect, definitionId);
        if (FAILED(hr)) {
            LogError("Failed to deregister sim vars.");
        } else {
            LogInfo("Sim vars deregistered.");
        }
    }
};


class SimManager {
public:
    static SimManager& Instance();  // Singleton accessor

    SimManager(const SimManager&) = delete;
    SimManager& operator=(const SimManager&) = delete;

    void Start();
    void Stop();

    void QueueTask(std::function<void()> task);

    void AddLiveVariable(const std::string& name, const std::string& unit);
    void AddFeedbackVariable(const std::string& name, const std::string& unit);
    void RemoveLiveVariables();
    void RemoveFeedbackVariables();

    std::optional<double> GetVariableValue(const std::string& name) const;

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

    SimGroup liveGroup_  = {1, 1, {}};
    SimGroup feedbackGroup_ = {2, 2, {}};

    std::unordered_map<std::string, double> variableValues_;
    mutable std::shared_mutex variableMutex_;
    void ParseGroupValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data, const SimGroup& group);
};
