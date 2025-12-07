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

class DialAction : public HSDAction {
public:
    using HSDAction::HSDAction;

    virtual void DidReceiveSettings(const nlohmann::json& payload) override;
    virtual void DialDown(const nlohmann::json& payload) override;
    virtual void DialUp(const nlohmann::json& payload) override;
    virtual void RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) override;
    virtual void RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) override;
    virtual void WillAppear(const nlohmann::json& payload) override;
    virtual void WillDisappear(const nlohmann::json& payload) override;
    void OnVariableUpdated(const std::string& name, double value);
    void UpdateImage();

    std::string displayValue_;

private:
    void UpdateVariablesAndEvents(const nlohmann::json& payload);
    void ClearSettings();

    const std::wstring b_Inactive = L"images/dial.png";
    const std::wstring b_Active = L"images/dial_active.png";
    const std::wstring ab_Inactive = L"images/dial_ab.png";
    const std::wstring ab_Active = L"images/dial_ab_active.png";

    // parsed settings
    std::string displayVar_;
    std::string feedbackVar_;
    std::string incEvent_;
    std::string decEvent_;
    std::string toggleEvent_;
    std::string header_;
    std::string skin_;
    bool isActive = false;

    SimVarDefinition displayVarDef_;
    SimVarDefinition feedbackVarDef_;
    SimEventDefinition incEventDef_;
    SimEventDefinition decEventDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId displaySubId_ = 0;
    SubscriptionId feedbackSubId_ = 0;
};
