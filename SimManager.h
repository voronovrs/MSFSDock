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
#include "SimGroup.h"

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

    mutable std::shared_mutex mutex_;
};
