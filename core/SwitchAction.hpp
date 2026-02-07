#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimData/SimVar.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/UIManager.hpp"

struct SwitchPosition {
    int simValue;
    std::string label;
};

class SwitchAction : public HSDAction, public IUIUpdatable {
public:
    using HSDAction::HSDAction;

    virtual void SendToPI(const nlohmann::json& payload) override;
    virtual void DidReceiveSettings(const nlohmann::json& payload) override;
    virtual void KeyDown(const nlohmann::json& payload) override;
    virtual void KeyUp(const nlohmann::json& payload) override;
    virtual void WillAppear(const nlohmann::json& payload) override;
    virtual void WillDisappear(const nlohmann::json& payload) override;
    void OnVariableUpdated(const std::string& name, double value);
    void UpdateImage();

private:
    void UpdateVariablesAndEvents(const nlohmann::json& payload);
    void ClearSettings();

    // parsed settings
    std::vector<SwitchPosition> positions_;
    std::unordered_map<int, int> valueToIndex_;

    std::string feedbackVar_;
    std::string toggleEvent_;
    std::string header_;
    int numPos_;
    int curPos_ = 0;

    SimVarDefinition feedbackVarDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId feedbackSubId_ = 0;
};
