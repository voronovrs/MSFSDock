#include "GDIPlusManager.h"
#include "Logger.h"
#include "ToggleAction.h"
#include "SimManager.h"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"

void ToggleAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("ToggleAction DidReceiveSettings. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);
    UpdateImage();
}

void ToggleAction::KeyDown(const nlohmann::json& payload) {
    LogInfo("ToggleAction KeyDown. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);

    toggle_settings.isActive = !toggle_settings.isActive;
    LogInfo((toggle_settings.isActive) ? "isactive true" : "isactive false");

    UpdateImage();
}

void ToggleAction::KeyUp(const nlohmann::json& payload) {
    // Log in release and debug builds
    LogInfo("ToggleAction KeyUp. Payload: " + payload.dump());
    // ShowOK();
    // Only log in debug builds (C++20-style format strings):
    // nlohmann::json settings = payload["settings"];
}

void ToggleAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("ToggleAction WillAppear. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);
    SimManager::Instance().AddLiveVariable(toggle_settings.displayVar, "feet");
    SimManager::Instance().AddFeedbackVariable(toggle_settings.feedbackVar, "number");
    UpdateImage();
}

void ToggleAction::WillDisappear(const nlohmann::json& payload) {
    LogInfo("ToggleAction WillDisappear. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);
    SimManager::Instance().RemoveLiveVariables();
    SimManager::Instance().RemoveFeedbackVariables();
    // if (settings.contains("header")) {
    // //     ShowAlert();
    //     SetTitle(settings["header"]);
    // }
}

void ToggleAction::SendToPlugin(const nlohmann::json& payload) {
    LogInfo("ToggleAction SendToPlugin. Payload: " + payload.dump());
    // SetTitle(payload["header"]);
    SetSettings(payload);
    nlohmann::json settings = payload["settings"];
    toggle_settings.FromJson(settings);
    // SimManager::Instance().RegisterVariable(toggle_settings.feedbackVar);
    // SimManager::Instance().RegisterVariable(toggle_settings.displayVar);
    UpdateImage();
}

void ToggleAction::UpdateImage() {
    std::wstring img_path = (toggle_settings.isActive) ? toggle_settings.backgroundImageActive : toggle_settings.backgroundImageInactive;
    std::string base64Image = DrawImage(img_path, toggle_settings.header, toggle_settings.displayVar);
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
