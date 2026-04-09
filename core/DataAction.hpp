#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BaseAction.hpp"
#include "ui/GDIPlusManager.hpp"


enum DataType {
    DATA_TYPE_ATTITUDE = 0,
    DATA_TYPE_HEADING,
    DATA_TYPE_SPEED,
    DATA_TYPE_ALTITUDE,
    DATA_TYPE_INFO,
};

class DataAction : public BaseAction {
public:

    DataAction(HSDConnectionManager* hsd_connection,
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

    std::string displayValue_;

private:
    void UpdateVariablesAndEvents(const nlohmann::json& payload);
    void ClearSettings();

    // parsed settings
    std::string header_;
    std::string header2_;
    DataType dataType_ = DATA_TYPE_INFO;

    std::string bgColor_ = "#141414";
    std::string outlineColor_ = "#a05f00";
    std::string headerColor_ = "#ebebeb";
    std::string dataColor_ = "#ebebeb";

    bool varIsInteger_ = true;
    bool var2IsInteger_ = true;

    std::chrono::steady_clock::time_point lastDrawTime_;
    double lastDrawPitch_ = 0.0f;
    double lastDrawBank_  = 0.0f;
    double lastDrawHeading_  = 0.0f;
    double lastDrawSpeed_  = 0.0f;
    double lastDrawAlt_  = 0.0f;
    double lastDrawInfo_  = 0.0f;
    double lastDrawInfo2_  = 0.0f;

    bool isButton_ = true;

    SimVarDefinition pitchVarDef_;
    SimVarDefinition bankVarDef_;
    SimVarDefinition headingVarDef_;
    SimVarDefinition speedVarDef_;
    SimVarDefinition altVarDef_;
    SimVarDefinition infoVarDef_;
    SimVarDefinition infoVar2Def_;

    SubscriptionId pitchSubId_ = 0;
    SubscriptionId bankSubId_ = 0;
    SubscriptionId headingSubId_ = 0;
    SubscriptionId speedSubId_ = 0;
    SubscriptionId altSubId_ = 0;
    SubscriptionId infoSubId_ = 0;
    SubscriptionId info2SubId_ = 0;
};
