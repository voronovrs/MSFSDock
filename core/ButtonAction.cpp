#include "ButtonAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "SimData/SimData.hpp"
#include <cmath>

void ButtonAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");
    skin_ = settings.value("skin", "skin1");

    // Conditional events configuration
    useConditionalEvents_ = settings.value("useConditionalEvents", false);
    conditionOperator_ = settings.value("conditionOperator", "==");
    conditionValue_ = settings.value("conditionValue", 0.0);
    eventWhenTrue_ = settings.value("eventWhenTrue", "");
    eventWhenFalse_ = settings.value("eventWhenFalse", "");

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    std::string newDisplay = settings.value("displayVar", "");
    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newConditional = settings.value("conditionalVar", "");
    std::string newEvent = settings.value("toggleEvent", "");

    // Remove variables if necessary
    if (!displayVar_.empty() && displayVar_ != newDisplay) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        varsToDeregister.push_back({ displayVar_, LIVE_VARIABLE });
    }

    if (!feedbackVar_.empty() && feedbackVar_ != newFeedback) {
        if (feedbackSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(feedbackVar_, feedbackSubId_);
        }
        varsToDeregister.push_back({ feedbackVar_, FEEDBACK_VARIABLE });
    }

    if (!conditionalVar_.empty() && conditionalVar_ != newConditional) {
        if (conditionalSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(conditionalVar_, conditionalSubId_);
        }
        varsToDeregister.push_back({ conditionalVar_, FEEDBACK_VARIABLE });
    }

    // Deregister old events
    if (!toggleEvent_.empty() && toggleEvent_ != newEvent) {
        eventsToDeregister.push_back({toggleEvent_});
    }

    std::string oldEventWhenTrue = eventWhenTrue_;
    std::string oldEventWhenFalse = eventWhenFalse_;
    
    if (!oldEventWhenTrue.empty() && oldEventWhenTrue != eventWhenTrue_) {
        eventsToDeregister.push_back({oldEventWhenTrue});
    }
    if (!oldEventWhenFalse.empty() && oldEventWhenFalse != eventWhenFalse_) {
        eventsToDeregister.push_back({oldEventWhenFalse});
    }

    // Add new variables if necessary
    if (!newDisplay.empty() && newDisplay != displayVar_) {
        displayVarDef_.name = newDisplay;
        displayVarDef_.group = LIVE_VARIABLE;
        varsToRegister.push_back(displayVarDef_);
    }

    if (!newFeedback.empty() && newFeedback != feedbackVar_) {
        feedbackVarDef_.name = newFeedback;
        feedbackVarDef_.group = FEEDBACK_VARIABLE;
        varsToRegister.push_back(feedbackVarDef_);
    }

    // Register conditional variable if needed
    if (useConditionalEvents_ && !newConditional.empty() && newConditional != conditionalVar_) {
        conditionalVarDef_.name = newConditional;
        conditionalVarDef_.group = FEEDBACK_VARIABLE;
        varsToRegister.push_back(conditionalVarDef_);
    }

    // Register events based on mode
    if (useConditionalEvents_) {
        if (!eventWhenTrue_.empty()) {
            eventWhenTrueDef_.name = eventWhenTrue_;
            eventsToRegister.push_back(eventWhenTrueDef_);
        }
        if (!eventWhenFalse_.empty()) {
            eventWhenFalseDef_.name = eventWhenFalse_;
            eventsToRegister.push_back(eventWhenFalseDef_);
        }
    } else {
        if (!newEvent.empty() && newEvent != toggleEvent_) {
            toggleEventDef_.name = newEvent;
            eventsToRegister.push_back(toggleEventDef_);
        }
    }

    // Call add/remove
    if (!varsToDeregister.empty())
        SimManager::Instance().RemoveSimVars(varsToDeregister);
    if (!eventsToDeregister.empty())
        SimManager::Instance().RemoveSimEvents(eventsToDeregister);

    if (!varsToRegister.empty())
        SimManager::Instance().AddSimVars(varsToRegister);
    if (!eventsToRegister.empty())
        SimManager::Instance().AddSimEvents(eventsToRegister);

    // Save new values
    displayVar_ = newDisplay;
    feedbackVar_ = newFeedback;
    conditionalVar_ = newConditional;
    toggleEvent_ = newEvent;

    // Subscribe callbacks
    if (!displayVar_.empty()) {
        displaySubId_ = SimManager::Instance().SubscribeToVariable(displayVar_,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
    if (!feedbackVar_.empty()) {
        feedbackSubId_ = SimManager::Instance().SubscribeToVariable(feedbackVar_,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
    if (useConditionalEvents_ && !conditionalVar_.empty()) {
        conditionalSubId_ = SimManager::Instance().SubscribeToVariable(conditionalVar_,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
}

void ButtonAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVar_) {
        displayVarDef_.value = value;
    }
    if (name == feedbackVar_) {
        isActive = (value != 0.0);
    }
    if (name == conditionalVar_) {
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
    if (!useConditionalEvents_) {
        return toggleEvent_;
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

    std::string result = conditionMet ? eventWhenTrue_ : eventWhenFalse_;
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
    out_payload["operators"] = nlohmann::json::array({"==", "!=", ">", "<", ">=", "<="});

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
    std::vector<SimVarDefinition> vars;
    std::vector<SimEventDefinition> events;
    
    if (!displayVar_.empty()) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        vars.push_back({displayVar_, LIVE_VARIABLE});
    }
    
    if (!feedbackVar_.empty()) {
        if (feedbackSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(feedbackVar_, feedbackSubId_);
        }
        vars.push_back({feedbackVar_, FEEDBACK_VARIABLE});
    }
    
    if (!conditionalVar_.empty()) {
        if (conditionalSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(conditionalVar_, conditionalSubId_);
        }
        vars.push_back({conditionalVar_, FEEDBACK_VARIABLE});
    }
    
    if (!vars.empty()) {
        SimManager::Instance().RemoveSimVars(vars);
    }

    if (!toggleEvent_.empty()) {
        events.push_back({toggleEvent_});
    }
    if (!eventWhenTrue_.empty()) {
        events.push_back({eventWhenTrue_});
    }
    if (!eventWhenFalse_.empty()) {
        events.push_back({eventWhenFalse_});
    }
    
    if (!events.empty()) {
        SimManager::Instance().RemoveSimEvents(events);
    }

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void ButtonAction::ClearSettings() {
    header_.clear();
    displayVar_.clear();
    feedbackVar_.clear();
    toggleEvent_.clear();
    conditionalVar_.clear();
    eventWhenTrue_.clear();
    eventWhenFalse_.clear();

    displaySubId_ = 0;
    feedbackSubId_ = 0;
    conditionalSubId_ = 0;
    useConditionalEvents_ = false;
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
    std::string val = (displayVar_.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));
    std::string base64Image = DrawButtonImage(img_path, header_, header_color, val, data_color,
        headerOffset, headerFontSize, dataOffset, dataFontSize, SimManager::Instance().IsConnected());
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
