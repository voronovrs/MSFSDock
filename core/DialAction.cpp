#include "DialAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"


void DialAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    std::string newDisplay = settings.value("displayVar", "");
    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newIncEvent = settings.value("incEvent", "");
    std::string newDecEvent = settings.value("decEvent", "");
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

    if (!incEvent_.empty() && incEvent_ != newIncEvent) {
        eventsToDeregister.push_back({incEvent_});
    }
    if (!decEvent_.empty() && decEvent_ != newDecEvent) {
        eventsToDeregister.push_back({decEvent_});
    }
    if (!toggleEvent_.empty() && toggleEvent_ != newEvent) {
        eventsToDeregister.push_back({toggleEvent_});
    }

    // Add new variables if necessary
    if (!newDisplay.empty()) {
        displayVarDef_.name = newDisplay;
        displayVarDef_.group = LIVE_VARIABLE;
        varsToRegister.push_back(displayVarDef_);
    }

    if (!newFeedback.empty()) {
        feedbackVarDef_.name = newFeedback;
        feedbackVarDef_.group = FEEDBACK_VARIABLE;
        varsToRegister.push_back(feedbackVarDef_);
    }

    if (!newIncEvent.empty()) {
        incEventDef_.name = newIncEvent;
        eventsToRegister.push_back(incEventDef_);
    }
    if (!newDecEvent.empty()) {
        decEventDef_.name = newDecEvent;
        eventsToRegister.push_back(decEventDef_);
    }
    if (!newEvent.empty()) {
        toggleEventDef_.name = newEvent;
        eventsToRegister.push_back(toggleEventDef_);
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
    incEvent_ = newIncEvent;
    decEvent_ = newDecEvent;
    toggleEvent_ = newEvent;

    // Subscribe callbacks
    if (!newDisplay.empty()) {
        displaySubId_ = SimManager::Instance().SubscribeToVariable(newDisplay,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
    if (!newFeedback.empty()) {
        feedbackSubId_ = SimManager::Instance().SubscribeToVariable(newFeedback,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
}

void DialAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVar_) {
        displayVarDef_.value = value;
    } else if (name == feedbackVar_) {
        isActive = (value == 0) ? false : true;
    }
    UpdateImage();
}

void DialAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("DialAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void DialAction::DialDown(const nlohmann::json& payload) {
    LogInfo("DialAction DialDown");
    // if (!decEvent_.empty()) {
        // SimManager::Instance().SendEvent(decEvent_);
    // }
}

void DialAction::DialUp(const nlohmann::json& payload) {
    LogInfo("DialAction DialUp");
    // if (!incEvent_.empty()) {
        // SimManager::Instance().SendEvent(incEvent_);
    // }
}

void DialAction::RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed){
    LogInfo("DialAction clockwise");
    if (!incEvent_.empty()) {
        SimManager::Instance().SendEvent(incEvent_);
    }
}

void DialAction::RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed){
    LogInfo("DialAction Counterclockwise");
    if (!decEvent_.empty()) {
        SimManager::Instance().SendEvent(decEvent_);
    }
}

void DialAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("DialAction WillAppear");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void DialAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("DialAction WillDisappear");
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
    if (!vars.empty()) {
        SimManager::Instance().RemoveSimVars(vars);
    }

    if (!incEvent_.empty()) {
        events.push_back({incEvent_});
    }
    if (!decEvent_.empty()) {
        events.push_back({decEvent_});
    }
    if (!toggleEvent_.empty()) {
        events.push_back({toggleEvent_});
    }
    if (!events.empty())
        SimManager::Instance().RemoveSimEvents(events);

    ClearSettings();
}

void DialAction::ClearSettings() {
    header_.clear();
    displayVar_.clear();
    feedbackVar_.clear();
    incEvent_.clear();
    decEvent_.clear();
    toggleEvent_.clear();

    displaySubId_ = 0;
    feedbackSubId_ = 0;
}

void DialAction::OnSimVarUpdated(const std::string& name, double value)
{
    if (name == displayVar_) {
        displayValue_ = std::to_string(value);
    }
    if (name == feedbackVar_) {
        isActive = (value != 0);
    }
    UpdateImage();
}

void DialAction::UpdateImage() {
    std::wstring img_path = (isActive) ? backgroundImageActive : backgroundImageInactive;
    int headerOffset = 0, headerFontSize = 0, dataOffset = 0, dataFontSize = 0, data2Offset = 0, data2FontSize = 0;
    std::string val = (displayVar_.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));

    headerOffset = -6;
    headerFontSize = 28;
    dataOffset = 32;
    dataFontSize = 46;
    std::string base64Image = DrawButtonImage(img_path, header_, val, "", headerOffset, headerFontSize, dataOffset, dataFontSize);

    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
