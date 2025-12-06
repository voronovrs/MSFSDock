#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimVar.hpp"

#ifdef SIM_MOCK
#include "sim/SimManagerMock.hpp"
#else
#include "sim/SimManager.hpp"
#endif

class ButtonAction : public HSDAction {
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
    void OnSimVarUpdated(const std::string& name, double value);

    const std::wstring backgroundImageInactive = L"images/button.png";
    const std::wstring backgroundImageActive = L"images/button_active.png";

    // parsed settings
    std::string displayVar_;
    std::string feedbackVar_;
    std::string toggleEvent_;
    std::string header_;
    bool isActive = false;

    SimVarDefinition displayVarDef_;
    SimVarDefinition feedbackVarDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId displaySubId_ = 0;
    SubscriptionId feedbackSubId_ = 0;
};
