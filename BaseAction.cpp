#include "BaseAction.h"
#include "SimVar.h"
#include "SimManager.h"

void BaseAction::RegisterValues() {
    std::vector<SimVarDefinition> variables = CollectVariables();
    std::vector<std::string> events = CollectEvents();

    SimManager::Instance().RegisterSimVars(variables);
    SimManager::Instance().DeregisterVariables();
    SimManager::Instance().RegisterVariables();
    SimManager::Instance().RegisterEvents(events);
}

void BaseAction::DeregisterValues() {
    std::vector<SimVarDefinition> variables = CollectVariables();;

    SimManager::Instance().DeregisterSimVars(variables);
    SimManager::Instance().DeregisterVariables();
    // SimManager::Instance().RegisterVariables();
}
