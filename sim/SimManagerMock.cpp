#include "SimManagerMock.hpp"
#include "plugin/Logger.hpp"

SimManager& SimManager::Instance() {
    static SimManager inst;
    return inst;
}

void SimManager::Start() {
    LogInfo("SimManager::Start (prototype) — no SimConnect");
}

void SimManager::Stop() {
    LogInfo("SimManager::Stop (prototype)");
}

void SimManager::RegisterSimVars(std::vector<SimVarDefinition>& vars) {
    std::lock_guard lock(mutex_);
    for (auto& v : vars) {
        if (v.IsValid()) {
            auto it = vars_.find(v.name);
            if (it == vars_.end()) {
                v.used = 1;
                vars_.emplace(v.name, v);
                LogInfo("SimManager: Add var: " + v.name + " group: " + std::to_string(v.group));
            } else {
                it->second.IncrementUsage();
                LogInfo("SimManager: Var already known: " + v.name  + " usage: " + std::to_string(it->second.used));
            }
        }
    }
}

void SimManager::DeregisterSimVars(const std::vector<SimVarDefinition>& vars) {
    std::lock_guard lock(mutex_);
    for (auto& v : vars) {
        auto it = vars_.find(v.name);
        if (it != vars_.end()) {
            it->second.DecrementUsage();
            LogInfo("SimManager: Decrement var usage: " + v.name + " usage: " + std::to_string(it->second.used));
            if (!it->second.IsInUse()) {
                vars_.erase(it);
                LogInfo("SimManager: Removed var: " + v.name);
            }
        } else {
            LogInfo("SimManager: Var to remove not found: " + v.name);
        }
    }
}

void SimManager::RegisterEvents(const std::vector<std::string>& events) {
    std::lock_guard lock(mutex_);
    for (auto& e : events) {
        if (!e.empty()) {
            auto it = events_.find(e);
            if (it == events_.end()) {
                events_.insert(e);
                LogInfo("SimManager: Add event: " + e);
            } else {
                LogInfo("SimManager: Event already known: " + e);
            }
        }
    }
}

void SimManager::DeregisterEvents(const std::vector<std::string>& events) {
    std::lock_guard lock(mutex_);
    for (auto& e : events) {
        auto it = events_.find(e);
        if (it != events_.end()) {
            events_.erase(it);
            LogInfo("SimManager: Removed event: " + e);
        } else {
            LogInfo("SimManager: Event to remove not found: " + e);
        }
    }
}

void SimManager::SendEvent(const std::string& name) {
    std::lock_guard lock(mutex_);
    if (events_.find(name) != events_.end()) {
        LogInfo("SimManager: (prototype) SendEvent: " + name);
    } else {
        LogWarn("SimManager: SendEvent: event not registered: " + name + " — still sending for prototype");
        LogInfo("SimManager: (prototype) SendEvent: " + name);
    }
}

std::vector<SimVarDefinition> SimManager::GetRegisteredVars() {
    std::lock_guard lock(mutex_);
    std::vector<SimVarDefinition> out;
    out.reserve(vars_.size());
    for (auto& kv : vars_) out.push_back(kv.second);
    return out;
}

std::vector<std::string> SimManager::GetRegisteredEvents() {
    std::lock_guard lock(mutex_);
    std::vector<std::string> out(events_.begin(), events_.end());
    return out;
}
