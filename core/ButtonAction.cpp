#include "ButtonAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"


void ButtonAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    std::string newDisplay = settings.value("displayVar", "");
    std::string newFeedback = settings.value("feedbackVar", "");
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

void ButtonAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVar_) {
        displayVarDef_.value = value;
    } else if (name == feedbackVar_) {
        isActive = (value == 0) ? false : true;
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
    if (!toggleEvent_.empty()) {
        SimManager::Instance().SendEvent(toggleEvent_);
    }
}

void ButtonAction::KeyUp(const nlohmann::json& /*payload*/) {
    // not used for now
}

void ButtonAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("ButtonAction WillAppear");
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
    if (!vars.empty()) {
        SimManager::Instance().RemoveSimVars(vars);
    }

    if (!toggleEvent_.empty()) {
        events.push_back({toggleEvent_});
        SimManager::Instance().RemoveSimEvents(events);
    }

    ClearSettings();
}

void ButtonAction::ClearSettings() {
    header_.clear();
    displayVar_.clear();
    feedbackVar_.clear();
    toggleEvent_.clear();

    displaySubId_ = 0;
    feedbackSubId_ = 0;
}

void ButtonAction::OnSimVarUpdated(const std::string& name, double value)
{
    if (name == displayVar_) {
        displayValue_ = std::to_string(value);
    }
    if (name == feedbackVar_) {
        isActive = (value != 0);
    }
    UpdateImage();
}

void ButtonAction::UpdateImage() {
    std::wstring img_path = (isActive) ? backgroundImageActive : backgroundImageInactive;
    std::string val = (displayVar_.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));
    std::string base64Image = DrawButtonImage(img_path, header_, val);
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
