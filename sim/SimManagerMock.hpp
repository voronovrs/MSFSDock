#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <set>

#include "core/SimVar.hpp"

// Prototype SimManager: does NOT call SimConnect — only logs and stores registered lists.

class SimManager {
public:
    static SimManager& Instance();

    void Start(); // no-op for prototype
    void Stop();  // no-op for prototype

    // Register/deregister lists coming from actions (prototype just stores + logs)
    void RegisterSimVars(std::vector<SimVarDefinition>& vars);
    void DeregisterSimVars(const std::vector<SimVarDefinition>& vars);

    void RegisterEvents(const std::vector<std::string>& events);
    void DeregisterEvents(const std::vector<std::string>& events);

    // For prototype emulate SendEvent by logging
    void SendEvent(const std::string& name);

    // Helpers to inspect current registry (for tests)
    std::vector<SimVarDefinition> GetRegisteredVars();
    std::vector<std::string> GetRegisteredEvents();

private:
    SimManager() = default;
    ~SimManager() = default;

    std::mutex mutex_;
    // Use maps/sets to avoid duplicates
    std::map<std::string, SimVarDefinition> vars_; // key = name
    std::set<std::string> events_;
};
