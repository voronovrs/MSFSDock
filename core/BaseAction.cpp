#include "BaseAction.hpp"

inline std::string to_hex32(uint32_t value)
{
    std::ostringstream ss;
    ss << std::hex
       << std::uppercase
       << std::setw(8)
       << std::setfill('0')
       << value;
    return ss.str();
}

BaseAction::BaseAction(HSDConnectionManager* hsd_connection,
                       const std::string& action,
                       const std::string& context)
    : HSDAction(hsd_connection, action, context)
{}

BaseAction::~BaseAction() = default;

void BaseAction::OnVariableUpdated(const std::string& name, double value) {
    // Default empty implementation
}

// Fill event parameters
void BaseAction::FillEvent(SimEventDefinition& e, const std::string& name,
                           EVENT_TYPES type, const std::array<uint32_t, 2>& actions) {
    if (name.empty())
        return;

    e.name = name;
    e.type = type;

    if (type == EVENT_PMDG) {
        e.pmdgID = GetPmdgEventID(name);
        e.pmdgActions = actions;
        e.uniqueName = name + "::" + to_hex32(actions[0]) + "::" + to_hex32(actions[1]);
    } else {
        e.pmdgActions = {0, 0};
        e.uniqueName = name;
    }
}

// Process variable changes
void BaseAction::DiffVar(VarBinding& v, std::vector<SimVarDefinition>& toAdd, std::vector<SimVarDefinition>& toRemove) {
    if (!v.def || !v.subId)
        return;

    const std::string& current = v.def->name;

    if (!current.empty() && current != v.next) {
        if (*v.subId) {
            SimManager::Instance().UnsubscribeFromVariable(current, *v.subId);
            *v.subId = 0;
        }
        toRemove.push_back({ current, v.group });
    }

    if (!v.next.empty() && current != v.next) {
        v.def->name = v.next;
        v.def->group = v.group;
        toAdd.push_back(*v.def);
    }
}

// Add variable callback
void BaseAction::SubscribeVar(VarBinding& v, const std::function<void(const std::string&, double)>& callback) {
    if (!v.subId)
        return;

    if (!v.next.empty()) {
        *v.subId = SimManager::Instance().SubscribeToVariable(v.next, callback);
    }
}

// Process event changes
void BaseAction::DiffEvent(EventBinding& e, std::vector<SimEventDefinition>& toAdd, std::vector<SimEventDefinition>& toRemove) {
    if (!e.def)
        return;

    if (!e.def->name.empty() && e.def->name != e.next) {
        toRemove.push_back({ e.def->name, e.def->uniqueName });
    }

    if (!e.next.empty() && e.def->name != e.next) {
        FillEvent(*e.def, e.next, e.type, e.actions);
        toAdd.push_back(*e.def);
    }
}

// Process events and variables changes within SimManager
void BaseAction::ApplyChanges(std::vector<SimVarDefinition>& varsToAdd,
                              std::vector<SimVarDefinition>& varsToRemove,
                              std::vector<SimEventDefinition>& eventsToAdd,
                              std::vector<SimEventDefinition>& eventsToRemove) {
    if (!varsToRemove.empty())
        SimManager::Instance().RemoveSimVars(varsToRemove);
    if (!eventsToRemove.empty())
        SimManager::Instance().RemoveSimEvents(eventsToRemove);

    if (!varsToAdd.empty())
        SimManager::Instance().AddSimVars(varsToAdd);
    if (!eventsToAdd.empty())
        SimManager::Instance().AddSimEvents(eventsToAdd);
}

// Apply configuration
void BaseAction::ApplyBindings() {
    varCallback_ = [this](const std::string& name, double value) {
        OnVariableUpdated(name, value);
    };

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    for (auto& v : varBindings_)
        DiffVar(v, varsToRegister, varsToDeregister);

    for (auto& e : eventBindings_)
        DiffEvent(e, eventsToRegister, eventsToDeregister);

    ApplyChanges(varsToRegister, varsToDeregister, eventsToRegister, eventsToDeregister);

    for (auto& v : varBindings_)
        SubscribeVar(v, varCallback_);
}

// Unregister all variables and events
void BaseAction::UnregisterAll() {
    std::vector<SimVarDefinition> varsToRemove;
    std::vector<SimEventDefinition> eventsToRemove;

    for (auto& v : varBindings_) {
        if (!v.def->name.empty()) {
            if (v.subId && *v.subId) {
                SimManager::Instance().UnsubscribeFromVariable(v.def->name, *v.subId);
                *v.subId = 0;
            }
            varsToRemove.push_back(*v.def);
        }
    }

    for (auto& e : eventBindings_) {
        if (!e.def->name.empty()) {
            eventsToRemove.push_back(*e.def);
        }
    }

    if (!varsToRemove.empty()) SimManager::Instance().RemoveSimVars(varsToRemove);
    if (!eventsToRemove.empty()) SimManager::Instance().RemoveSimEvents(eventsToRemove);
}

// Cleanup
void BaseAction::CleanUp() {
    for (auto& v : varBindings_) {
        v.def->name.clear();
    }

    for (auto& e : eventBindings_) {
        e.def->name.clear();
    }
}

nlohmann::json BaseAction::BuildCommonPayloadJson(bool isPmdg) const {
    nlohmann::json out;
    out["type"] = "evt_var_list";
    out["common_events"] = nlohmann::json::array();
    out["common_variables"] = nlohmann::json::array();

    if (isPmdg) {
        for (const auto& v : GetPmdgVariables())
            out["common_variables"].push_back(v);

        for (const auto& e : GetPmdgEvents())
            out["common_events"].push_back(e);
    } else {
        for (const auto& v : GetKnownVariables())
            out["common_variables"].push_back(v);

        for (const auto& e : GetKnownEvents())
            out["common_events"].push_back(e);
    }

    return out;
}
