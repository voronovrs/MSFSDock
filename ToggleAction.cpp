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
}

void ToggleAction::DidReceiveSettings(const nlohmann::json& payload) {
    // LogInfo("ToggleAction DidReceiveSettings. Payload: " + payload.dump());
    InitializeSettings(payload);
    UpdateImage();
}

void ToggleAction::KeyDown(const nlohmann::json& payload) {
    // LogInfo("ToggleAction KeyDown. Payload: " + payload.dump());
    toggle_settings.isActive = !toggle_settings.isActive;
    UpdateImage();
}

void ToggleAction::KeyUp(const nlohmann::json& payload) {
    // Log in release and debug builds
    // LogInfo("ToggleAction KeyUp. Payload: " + payload.dump());
    // ShowOK();
    // Only log in debug builds (C++20-style format strings):
    // nlohmann::json settings = payload["settings"];
}

void ToggleAction::WillAppear(const nlohmann::json& payload) {
    // LogInfo("ToggleAction WillAppear. Payload: " + payload.dump());
    InitializeSettings(payload);
    SimManager::Instance().SubscribeToVariable(toggle_settings.displayVar,
        [this](const std::string& name, double value) {
            // Throttle if needed
            if (toggle_settings.displayValue != std::to_string(static_cast<int>(value))) {
                toggle_settings.displayValue = std::to_string(static_cast<int>(value));
                UpdateImage();
            }
        });

    UpdateImage();
}

void ToggleAction::WillDisappear(const nlohmann::json& payload) {
    // LogInfo("ToggleAction WillDisappear. Payload: " + payload.dump());
    DeregisterValues();
    UninitializeSettings();
}

void ToggleAction::SendToPlugin(const nlohmann::json& payload) {
    // LogInfo("ToggleAction SendToPlugin. Payload: " + payload.dump());
    // SetTitle(payload["header"]);
    // SetSettings(payload);
    // nlohmann::json settings = payload["settings"];
    // toggle_settings.FromJson(settings);
    // SimManager::Instance().RegisterVariable(toggle_settings.feedbackVar);
    // SimManager::Instance().RegisterVariable(toggle_settings.displayVar);
    InitializeSettings(payload);
    UpdateImage();
}

void ToggleAction::UpdateImage() {
    std::wstring img_path = (toggle_settings.isActive) ? toggle_settings.backgroundImageActive : toggle_settings.backgroundImageInactive;
    std::string base64Image = DrawImage(img_path, toggle_settings.header, (!toggle_settings.displayVar.empty()) ? toggle_settings.displayValue : "");
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
