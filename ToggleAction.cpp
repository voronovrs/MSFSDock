#include "GDIPlusManager.h"
#include "Logger.h"
#include "ToggleAction.h"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"

std::vector<SimVarDefinition> ToggleAction::CollectVariables() {
    std::vector<SimVarDefinition> variables;
    if (toggle_settings.displayReg.IsValid()) {
        variables.push_back(toggle_settings.displayReg);
    }
    if (toggle_settings.feedbackReg.IsValid()) {
        variables.push_back(toggle_settings.feedbackReg);
    }
    return variables;
}

void ToggleAction::InitializeSettings(const nlohmann::json& payload) {
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);

    SimVarDefinition newDisplayReg;
    SimVarDefinition newFeedbackReg;

    if (!toggle_settings.displayVar.empty()) {
        newDisplayReg = {.name=toggle_settings.displayVar, .group=LIVE_VARIABLE};
    }
    if (!toggle_settings.feedbackVar.empty()) {
        newFeedbackReg = {.name=toggle_settings.feedbackVar, .group=FEEDBACK_VARIABLE};
    }

    if (!toggle_settings.toggleEvent.empty()) {
        toggle_settings.toggleEventID = SimManager::Instance().RegisterEvent(toggle_settings.toggleEvent);
    }

    if (newDisplayReg == toggle_settings.displayReg && newFeedbackReg == toggle_settings.feedbackReg) {
        // Change in non-meaningfull fields, no need to update variables
        return;
    }

    DeregisterValues();
    toggle_settings.displayReg = newDisplayReg;
    toggle_settings.feedbackReg = newFeedbackReg;
    RegisterValues();
}

void ToggleAction::UninitializeSettings() {
    toggle_settings.displayReg = {};
    toggle_settings.feedbackReg = {};
    toggle_settings.displayValue = "";
    toggle_settings.isActive = false;
}

void ToggleAction::DidReceiveSettings(const nlohmann::json& payload) {
    // LogInfo("ToggleAction DidReceiveSettings. Payload: " + payload.dump());
    InitializeSettings(payload);
    UpdateImage();
}

void ToggleAction::KeyDown(const nlohmann::json& payload) {
    if (!toggle_settings.toggleEvent.empty()) {
        SimManager::Instance().SendEvent(toggle_settings.toggleEvent);
    }
}

void ToggleAction::KeyUp(const nlohmann::json& payload) {
    // LogInfo("ToggleAction KeyUp. Payload: " + payload.dump());
}

void ToggleAction::WillAppear(const nlohmann::json& payload) {
    // LogInfo("ToggleAction WillAppear. Payload: " + payload.dump());
    InitializeSettings(payload);
    SimManager::Instance().SubscribeToVariable(toggle_settings.displayVar,
        [this](const std::string& name, double value) {
            // Throttle if needed
            if (!toggle_settings.displayVar.empty()) {
                if (toggle_settings.displayValue != std::to_string(static_cast<int>(value))) {
                    toggle_settings.displayValue = std::to_string(static_cast<int>(value));
                    UpdateImage();
                }
            }
        });

    SimManager::Instance().SubscribeToVariable(toggle_settings.feedbackVar,
        [this](const std::string& name, double value) {
            // Throttle if needed
            if (!toggle_settings.feedbackVar.empty()) {
                toggle_settings.isActive = (static_cast<int>(value) != 0);
                UpdateImage();
            }
        });

    double initialVal;
    if (SimManager::Instance().TryGetCachedValue(toggle_settings.displayVar, initialVal)) {
        toggle_settings.displayValue = std::to_string((int)initialVal);
    }

    if (SimManager::Instance().TryGetCachedValue(toggle_settings.feedbackVar, initialVal)) {
        toggle_settings.isActive = (initialVal != 0);
    }

    UpdateImage();
}

void ToggleAction::WillDisappear(const nlohmann::json& payload) {
    // LogInfo("ToggleAction WillDisappear. Payload: " + payload.dump());
    DeregisterValues();
    UninitializeSettings();
}

void ToggleAction::SendToPlugin(const nlohmann::json& payload) {
    InitializeSettings(payload);
    UpdateImage();
}

void ToggleAction::UpdateImage() {
    std::wstring img_path = (toggle_settings.isActive) ? toggle_settings.backgroundImageActive : toggle_settings.backgroundImageInactive;
    std::string base64Image = DrawButtonImage(img_path, toggle_settings.header, (!toggle_settings.displayVar.empty()) ? toggle_settings.displayValue : "");
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
