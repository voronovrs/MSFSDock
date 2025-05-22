#include "SimManager.h"
#include "Logger.h"


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
    LogInfo("Recieved dwID " + std::to_string(pData->dwID));
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_QUIT:
            LogWarn("MSFS has closed or disconnected.");
            // Signal your thread to exit or try reconnect
            manager->Stop();
            break;

        // Handle other message types here

        default:
            break;
    }
}

void SimManager::RegisterVariable(std::string name, std::string unit) {
    QueueTask([=, this] {
        std::unique_lock lock(mutex_);
        if (variableNameToId_.contains(name))
            return; // Already registered

        uint32_t varId = nextVarId_++;
        variableNameToId_[name] = varId;
        idToVariableName_[varId] = name;

        HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, varId, name.c_str(), unit.c_str());
        if (FAILED(hr)) {
            LogError("Failed to register sim variable: " + name);
        } else {
            LogInfo("Registered sim variable: " + name + " -> ID " + std::to_string(varId));
        }
    });
}

std::optional<uint32_t> SimManager::GetVariableId(const std::string& name) const {
    std::shared_lock lock(mutex_);
    auto it = variableNameToId_.find(name);
    if (it != variableNameToId_.end())
        return it->second;
    return std::nullopt;
}

std::optional<std::string> SimManager::GetVariableName(uint32_t id) const {
    std::shared_lock lock(mutex_);
    auto it = idToVariableName_.find(id);
    if (it != idToVariableName_.end())
        return it->second;
    return std::nullopt;
}
