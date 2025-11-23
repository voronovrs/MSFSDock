#pragma once

#include "SimVar.h"
#include "SimManager.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"

class BaseAction {
public:
    virtual void InitializeSettings(const nlohmann::json& payload) = 0;
    virtual void UninitializeSettings() = 0;
    virtual void UpdateImage() = 0;

    virtual void RegisterValues();
    virtual void DeregisterValues();

    virtual ~BaseAction() = default;
private:
    virtual std::vector<SimVarDefinition> CollectVariables() = 0;
    virtual std::vector<std::string> CollectEvents() = 0;
};
