#include "ButtonAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "Utils.hpp"
#include <cmath>

namespace EVT = BaseActionEvents;

void ButtonAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");
    skin_ = settings.value("skin", "skin1");

    conditionOperator_ = settings.value("conditionOperator", "==");
    conditionValue_ = getFloatFromJson(settings, "conditionValue", 0.0f);

    std::string newDisplay          = settings.value("displayVar", "");
    std::string newFeedback         = settings.value("feedbackVar", "");
    std::string newConditionalVar   = settings.value("conditionalVar", "");
    std::string newEvent            = settings.value("toggleEvent", "");
    std::string newEventWhenTrue    = settings.value("eventWhenTrue", "");
    std::string newEventWhenFalse   = settings.value("eventWhenFalse", "");

    varBindings_ = {
        {&displayVarDef_, newDisplay, LIVE_VARIABLE, &displaySubId_},
        {&feedbackVarDef_, newFeedback, FEEDBACK_VARIABLE, &feedbackSubId_},
        {&conditionalVarDef_, newConditionalVar, FEEDBACK_VARIABLE, &conditionalSubId_},
    };

    eventBindings_ = {
        { &toggleEventDef_, newEvent, EVENT_GENERIC, EVT::GENERIC },
        { &eventWhenTrueDef_, newEventWhenTrue, EVENT_GENERIC, EVT::GENERIC },
        { &eventWhenFalseDef_, newEventWhenFalse, EVENT_GENERIC, EVT::GENERIC },
    };

    ApplyBindings();
}

void ButtonAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVarDef_.name) {
        displayVarDef_.value = value;
    }
    if (name == feedbackVarDef_.name) {
        isActive = (value != 0.0);
    }
    if (name == conditionalVarDef_.name) {
        conditionalVarDef_.value = value;
    }
    UpdateImage();
}

void ButtonAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("ButtonAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void ButtonAction::KeyDown(const nlohmann::json& payload) {
    LogInfo("ButtonAction KeyDown");
    if (!SimManager::Instance().IsConnected()) {
        SimManager::Instance().EnsureConnected();
        return;
    }

    std::string eventToSend = GetEventToSend();
    if (!eventToSend.empty()) {
        LogInfo("Sending event: " + eventToSend);
        SimManager::Instance().SendEvent(eventToSend);
    } else {
        LogInfo("No event to send (empty event name)");
    }
}

void ButtonAction::KeyUp(const nlohmann::json& /*payload*/) {
    // not used for now
}

std::string ButtonAction::GetEventToSend() const {
    if (!isConditional) {
        return toggleEventDef_.name;
    }

    double varValue = conditionalVarDef_.value;
    bool conditionMet = false;

    if (conditionOperator_ == "==") {
        conditionMet = (std::abs(varValue - conditionValue_) < 0.0001);
    } else if (conditionOperator_ == "!=") {
        conditionMet = (std::abs(varValue - conditionValue_) >= 0.0001);
    } else if (conditionOperator_ == ">") {
        conditionMet = (varValue > conditionValue_);
    } else if (conditionOperator_ == "<") {
        conditionMet = (varValue < conditionValue_);
    } else if (conditionOperator_ == ">=") {
        conditionMet = (varValue >= conditionValue_);
    } else if (conditionOperator_ == "<=") {
        conditionMet = (varValue <= conditionValue_);
    }

    std::string result = conditionMet ? eventWhenTrueDef_.name : eventWhenFalseDef_.name;
    LogInfo("Condition evaluation: varValue=" + std::to_string(varValue) +
            " conditionValue=" + std::to_string(conditionValue_) +
            " operator=" + conditionOperator_ +
            " result=" + (conditionMet ? "TRUE" : "FALSE") +
            " event=" + result);

    return result;
}

void ButtonAction::SendToPI(const nlohmann::json& payload) {
    nlohmann::json out_payload;
    out_payload["type"] = "evt_var_list";
    out_payload["common_events"] = nlohmann::json::array();
    out_payload["common_variables"] = nlohmann::json::array();

    for (const auto& evt : GetKnownVariables()) {
        out_payload["common_variables"].push_back(evt);
    }

    for (const auto& evt : GetKnownEvents()) {
        out_payload["common_events"].push_back(evt);
    }

    SendToPropertyInspector(out_payload);
}

void ButtonAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("ButtonAction WillAppear");
    UIManager::Instance().Register(this);
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void ButtonAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("ButtonAction WillDisappear");

    UnregisterAll();

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void ButtonAction::ClearSettings() {
    header_.clear();

    CleanUp();

    conditionValue_ = 0.0;
    conditionOperator_ = "==";
}

void ButtonAction::UpdateImage() {
    int headerOffset = 0, headerFontSize = 0, dataOffset = 0, dataFontSize = 0;
    std::wstring backgroundImageInactive, backgroundImageActive;
    Gdiplus::Color header_color, data_color;

    if (skin_ == "skin1") {
        backgroundImageInactive = b_Inactive;
        backgroundImageActive = b_Active;
        headerOffset = 4;
        headerFontSize = 16;
        dataOffset = 25;
        dataFontSize = 20;
        header_color = COLOR_WHITE;
        data_color = COLOR_OFF_WHITE;
    } else {
        backgroundImageInactive = ab_Inactive;
        backgroundImageActive = ab_Active;
        headerOffset = 44;
        headerFontSize = 16;
        dataOffset = 25;
        dataFontSize = 20;
        header_color = COLOR_BRIGHT_ORANGE;
        data_color = COLOR_OFF_WHITE;
    }

    std::wstring img_path = (isActive) ? backgroundImageActive : backgroundImageInactive;
    std::string val = (displayVarDef_.name.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));
    std::string base64Image = DrawButtonImage(img_path, header_, header_color, val, data_color,
        headerOffset, headerFontSize, dataOffset, dataFontSize, SimManager::Instance().IsConnected());
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
