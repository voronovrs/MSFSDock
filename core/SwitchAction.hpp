#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BaseAction.hpp"

struct SwitchPosition {
    int simValue;
    std::string label;
};

class SwitchAction : public BaseAction {
public:

    SwitchAction(HSDConnectionManager* hsd_connection,
                const std::string& action,
                const std::string& context)
        : BaseAction(hsd_connection, action, context)
    {}

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

    std::string header_;
    int numPos_;
    int curPos_ = 0;

    SimVarDefinition feedbackVarDef_;
    SimEventDefinition toggleEventDef_;

    SubscriptionId feedbackSubId_ = 0;
};
