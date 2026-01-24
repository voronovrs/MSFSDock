#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimVar.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/UIManager.hpp"

class ButtonAction : public HSDAction, public IUIUpdatable {
public:
    using HSDAction::HSDAction;

    virtual void DidReceiveSettings(const nlohmann::json& payload) override;
    virtual void KeyDown(const nlohmann::json& payload) override;
    virtual void KeyUp(const nlohmann::json& payload) override;
    virtual void WillAppear(const nlohmann::json& payload) override;
    virtual void WillDisappear(const nlohmann::json& payload) override;
    void OnVariableUpdated(const std::string& name, double value);
    void UpdateImage();

    std::string displayValue_;

private:
    void UpdateVariablesAndEvents(const nlohmann::json& payload);
    void ClearSettings();

    // parsed settings
    std::string displayVar_;
    std::string feedbackVar_;
    std::string toggleEvent_;
    std::string header_;
    bool isActive = false;
    std::string skin_;

    const std::wstring b_Inactive = L"images/button.png";
    const std::wstring b_Active = L"images/button_active.png";
    const std::wstring ab_Inactive = L"images/button_ab.png";
    const std::wstring ab_Active = L"images/button_ab_active.png";

    SimVarDefinition displayVarDef_;
    SimVarDefinition feedbackVarDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId displaySubId_ = 0;
    SubscriptionId feedbackSubId_ = 0;
};
