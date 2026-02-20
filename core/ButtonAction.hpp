#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BaseAction.hpp"

class ButtonAction : public BaseAction {
public:

    ButtonAction(HSDConnectionManager* hsd_connection,
                const std::string& action,
                const std::string& context,
                bool isConditional, bool isPmdg)
        : BaseAction(hsd_connection, action, context),
          isConditional(isConditional), isPmdg(isPmdg)
    {}

    virtual void SendToPI(const nlohmann::json& payload) override;
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
    std::string GetEventToSend() const;

    // parsed settings
    bool isActive = false;
    std::string header_;
    std::string skin_;

    bool isPmdg = false;

    // Conditional events support
    bool isConditional = false;
    std::string conditionOperator_;
    double conditionValue_ = 0.0;

    const std::wstring b_Inactive = L"images/button.png";
    const std::wstring b_Active = L"images/button_active.png";
    const std::wstring ab_Inactive = L"images/button_ab.png";
    const std::wstring ab_Active = L"images/button_ab_active.png";

    SimVarDefinition displayVarDef_;
    SimVarDefinition feedbackVarDef_;
    SimEventDefinition toggleEventDef_;
    SimVarDefinition conditionalVarDef_;
    SimEventDefinition eventWhenTrueDef_;
    SimEventDefinition eventWhenFalseDef_;

    SubscriptionId displaySubId_ = 0;
    SubscriptionId feedbackSubId_ = 0;
    SubscriptionId conditionalSubId_ = 0;
};
