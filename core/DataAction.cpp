#include "DataAction.hpp"
#include "plugin/Logger.hpp"
#include "Utils.hpp"
#include <numbers>

namespace EVT = BaseActionEvents;

static DataType GetDataType(const std::string& value) {
    if (value == "attitude") return DATA_TYPE_ATTITUDE;
    if (value == "heading")  return DATA_TYPE_HEADING;
    if (value == "speed")    return DATA_TYPE_SPEED;
    if (value == "alt")      return DATA_TYPE_ALTITUDE;
    if (value == "info")     return DATA_TYPE_INFO;

    return DATA_TYPE_INFO;
}

void DataAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings"))
        return;

    const auto& settings = payload["settings"];

    dataType_ = GetDataType(settings.value("type", "info"));

    std::string pitchVar = (dataType_ == DATA_TYPE_ATTITUDE) ? "PLANE PITCH DEGREES" : "";
    std::string bankVar = (dataType_ == DATA_TYPE_ATTITUDE) ? "PLANE BANK DEGREES" : "";
    std::string headingVar = (dataType_ == DATA_TYPE_HEADING) ? "PLANE HEADING DEGREES MAGNETIC" : "";
    std::string speedVar = (dataType_ == DATA_TYPE_SPEED) ? "AIRSPEED INDICATED" : "";
    std::string altVar = (dataType_ == DATA_TYPE_ALTITUDE) ? "INDICATED ALTITUDE" : "";
    std::string infoVar = (dataType_ == DATA_TYPE_INFO) ? settings.value("infoVar", "") : "";
    std::string infoVar2 = (dataType_ == DATA_TYPE_INFO) ? settings.value("infoVar2", "") : "";

    if (dataType_ == DATA_TYPE_INFO) {
        header_ = settings.value("header", "");
        header2_ = settings.value("header2", "");

        bgColor_ = settings.value("bgColor", "#141414");
        outlineColor_ = settings.value("outlineColor", "#a05f00");
        headerColor_ = settings.value("headerColor", "#ebebeb");
        dataColor_ = settings.value("dataColor", "#ebebeb");

        varIsInteger_ = settings.value("varFormat", "integer") == "integer";
        var2IsInteger_ = settings.value("varFormat2", "integer") == "integer";
    }

    varBindings_ = {
        {&pitchVarDef_, pitchVar, LIVE_VARIABLE, &pitchSubId_},
        {&bankVarDef_, bankVar, LIVE_VARIABLE, &bankSubId_},
        {&headingVarDef_, headingVar, LIVE_VARIABLE, &headingSubId_},
        {&speedVarDef_, speedVar, LIVE_VARIABLE, &speedSubId_},
        {&altVarDef_, altVar, LIVE_VARIABLE, &altSubId_},
        {&infoVarDef_, infoVar, LIVE_VARIABLE, &infoSubId_},
        {&infoVar2Def_, infoVar2, LIVE_VARIABLE, &info2SubId_},
        };

    ApplyBindings();
}

void DataAction::OnVariableUpdated(const std::string& name, double value) {
    const double step = 0.1;

    if (name == pitchVarDef_.name) {
        pitchVarDef_.value = RoundToStep(value, step);
    } else if (name == bankVarDef_.name) {
        bankVarDef_.value = RoundToStep(value, step);
    } else if (name == headingVarDef_.name) {
        headingVarDef_.value = RoundToStep(value, step);
    } else if (name == speedVarDef_.name) {
        speedVarDef_.value = RoundToStep(value, step);
    } else if (name == altVarDef_.name) {
        altVarDef_.value = RoundToStep(value, 10.0);
    } else if (name == infoVarDef_.name) {
        infoVarDef_.value = RoundToStep(value, 0.001);
        LogInfo("infoVarDef_ " + std::to_string(infoVarDef_.value));
    } else if (name == infoVar2Def_.name) {
        infoVar2Def_.value = RoundToStep(value, 0.001);
        LogInfo("infoVar2Def_ " + std::to_string(infoVar2Def_.value));
    }

    auto now = std::chrono::steady_clock::now();

    bool dataChanged = (pitchVarDef_.value != lastDrawPitch_) || (bankVarDef_.value  != lastDrawBank_) ||
                       (headingVarDef_.value != lastDrawHeading_) || (speedVarDef_.value  != lastDrawSpeed_) ||
                       (altVarDef_.value  != lastDrawAlt_) || (infoVarDef_.value  != lastDrawInfo_) ||
                       (infoVar2Def_.value  != lastDrawInfo2_);

    if (!dataChanged)
        return;

    // if (now - lastDrawTime_ < std::chrono::milliseconds(33))  // ~30FPS
    //     return;

    lastDrawPitch_ = pitchVarDef_.value;
    lastDrawBank_ = bankVarDef_.value;
    lastDrawHeading_ = headingVarDef_.value;
    lastDrawSpeed_ = speedVarDef_.value;
    lastDrawAlt_ = altVarDef_.value;
    lastDrawInfo_ = infoVarDef_.value;
    lastDrawInfo2_ = infoVar2Def_.value;
    lastDrawTime_ = now;

    UpdateImage();
}

void DataAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("DataAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void DataAction::KeyDown(const nlohmann::json& payload) {
    LogInfo("DataAction KeyDown");
    if (!SimManager::Instance().IsConnected()) {
        SimManager::Instance().EnsureConnected();
    }
}

void DataAction::KeyUp(const nlohmann::json& /*payload*/) {
    // not used for now
}

void DataAction::SendToPI(const nlohmann::json& payload) {
    nlohmann::json out_payload = BuildCommonPayloadJson();

    SendToPropertyInspector(out_payload);
}

void DataAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("DataAction WillAppear");
    UIManager::Instance().Register(this);
    UpdateVariablesAndEvents(payload);
    LogInfo(payload.value("controller", ""));
    isButton_ = (payload.value("controller", "Keypad") == "Keypad") ? true : false;

    UpdateImage();
}

void DataAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("DataAction WillDisappear");
    UnregisterAll();

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void DataAction::ClearSettings() {
    header_.clear();
    header2_.clear();
    CleanUp();
}

void DataAction::UpdateImage() {
    std::string base64Image;

    if (dataType_ == DATA_TYPE_ATTITUDE)
        base64Image = DrawHorizon(pitchVarDef_.value, bankVarDef_.value, (isButton_) ? SCREEN_TYPE_BUTTON : SCREEN_TYPE_DISPLAY, SimManager::Instance().IsConnected());
    else if (dataType_ == DATA_TYPE_HEADING)
        base64Image = DrawHeading(headingVarDef_.value, (isButton_) ? SCREEN_TYPE_BUTTON : SCREEN_TYPE_DISPLAY, SimManager::Instance().IsConnected());
    else if (dataType_ == DATA_TYPE_SPEED)
        base64Image = DrawRibons(speedVarDef_.value, true, (isButton_) ? SCREEN_TYPE_BUTTON : SCREEN_TYPE_DISPLAY, SimManager::Instance().IsConnected());
    else if (dataType_ == DATA_TYPE_ALTITUDE)
        base64Image = DrawRibons(altVarDef_.value, false, (isButton_) ? SCREEN_TYPE_BUTTON : SCREEN_TYPE_DISPLAY, SimManager::Instance().IsConnected());
    else if (dataType_ == DATA_TYPE_INFO) {
        std::string infoVal = (infoVarDef_.name.empty()) ? "" : doubleToStr(infoVarDef_.value, varIsInteger_);
        std::string info2Val = (infoVar2Def_.name.empty()) ? "" : doubleToStr(infoVar2Def_.value, var2IsInteger_);
        base64Image = DrawInfo(bgColor_, outlineColor_, headerColor_, dataColor_, header_, infoVal, header2_, info2Val, (isButton_) ? SCREEN_TYPE_BUTTON : SCREEN_TYPE_DISPLAY, SimManager::Instance().IsConnected());
    } else {
        LogWarn("Not implemented");
        return;
    }
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
