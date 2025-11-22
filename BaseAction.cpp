#include "BaseAction.h"
#include "SimVar.h"
#include "SimManager.h"

void BaseAction::RegisterValues() {
    std::vector<SimVarDefinition> variables = CollectVariables();

    SimManager::Instance().RegisterSimVars(variables);
    SimManager::Instance().DeregisterVariables();
    SimManager::Instance().RegisterVariables();
}

void BaseAction::DeregisterValues() {
    std::vector<SimVarDefinition> variables = CollectVariables();;

    SimManager::Instance().DeregisterSimVars(variables);
    SimManager::Instance().DeregisterVariables();
    SimManager::Instance().RegisterVariables();
}
