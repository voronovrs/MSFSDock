#pragma once

#include <Windows.h>
#include <SimConnect.h>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <optional>
#include <functional>
#include <condition_variable>

class SimManager {
public:
    SimManager();
    ~SimManager();

    void Start();
    void Stop();

    void QueueTask(std::function<void()> task);

    void RegisterVariable(std::string name, std::string unit = "number");
    // Future extensions:
    // void TryGetVariable(std::string name);
    // void RegisterEvent(std::string name, std::string status);
    // void DeregisterVariables();
    // void DeregisterEvents();
    std::optional<uint32_t> GetVariableId(const std::string& name) const;
    std::optional<std::string> GetVariableName(uint32_t id) const;

private:
    void Run();
    static void CALLBACK DispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

    HANDLE hSimConnect = nullptr;
    std::thread simThread;
    std::atomic<bool> running = false;

    std::queue<std::function<void()>> simTasks;
    std::mutex simTaskMutex;
    std::condition_variable simTaskCV;

    std::unordered_map<std::string, uint32_t> variableNameToId_;
    std::unordered_map<std::string, uint32_t> eventNameToId_;
    std::unordered_map<uint32_t, std::string> idToVariableName_;
    std::unordered_map<uint32_t, std::string> idToEventName_;
    uint32_t nextVarId_ = 0;
    uint32_t nextEventId_ = 0;

    mutable std::shared_mutex mutex_;
};