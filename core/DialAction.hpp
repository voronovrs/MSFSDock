#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BaseAction.hpp"

class DialAction : public BaseAction {
public:

    DialAction(HSDConnectionManager* hsd_connection,
               const std::string& action,
               const std::string& context,
               bool isDual, bool isRadio)
        : BaseAction(hsd_connection, action, context),
          isDual(isDual), isRadio(isRadio)
    {}

    virtual void SendToPI(const nlohmann::json& payload) override;
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
    const std::wstring b_Dual_1 = L"images/dualdial_1.png";
    const std::wstring b_Dual_2 = L"images/dualdial_2.png";
    const std::wstring ab_Dual_1 = L"images/dualdial_ab_1.png";
    const std::wstring ab_Dual_2 = L"images/dualdial_ab_2.png";

    // parsed settings
    std::string header_;
    std::string skin_;

    bool isActive = false;
    bool isDual = false;
    bool isRadio = false;
    int active_dial = 0;
    int active_radio_part = 0;

    std::chrono::steady_clock::time_point lastClickTs_;
    bool clickPending_ = false;

    SimVarDefinition displayVarDef_;
    SimVarDefinition display2VarDef_;
    SimVarDefinition feedbackVarDef_;
    SimEventDefinition incEventDef_;
    SimEventDefinition decEventDef_;
    SimEventDefinition inc2EventDef_;
    SimEventDefinition dec2EventDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId displaySubId_ = 0;
    SubscriptionId display2SubId_ = 0;
    SubscriptionId feedbackSubId_ = 0;
};
