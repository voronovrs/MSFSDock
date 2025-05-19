#include "GDIPlusManager.h"
#include "Logger.h"
#include "DialAction.h"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"

void DialAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("DialAction DidReceiveSettings. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    dial_settings.FromJson(settings);
    UpdateImage();
}

void DialAction::DialDown(const nlohmann::json& payload) {
    LogInfo("DialAction DialDown. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    dial_settings.FromJson(settings);

    dial_settings.isActive = !dial_settings.isActive;
    LogInfo((dial_settings.isActive) ? "isactive true" : "isactive false");

    UpdateImage();
}

void DialAction::DialUp(const nlohmann::json& payload) {
    // Log in release and debug builds
    LogInfo("DialAction DialUp. Payload: " + payload.dump());
    // ShowOK();
    // Only log in debug builds (C++20-style format strings):
    // nlohmann::json settings = payload["settings"];
}

void DialAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("DialAction WillAppear. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    dial_settings.FromJson(settings);
    UpdateImage();
}

void DialAction::WillDisappear(const nlohmann::json& payload) {
    LogInfo("DialAction WillDisappear. Payload: " + payload.dump());
    nlohmann::json settings = payload["settings"];
    // if (settings.contains("header")) {
    // //     ShowAlert();
    //     SetTitle(settings["header"]);
    // }
}

void DialAction::SendToPlugin(const nlohmann::json& payload) {
    LogInfo("DialAction SendToPlugin. Payload: " + payload.dump());
    // SetTitle(payload["header"]);
    SetSettings(payload);
}

void DialAction::UpdateImage() {
    int headerOffset = 0, headerFontSize = 0, dataOffset = 0, dataFontSize = 0, data2Offset = 0, data2FontSize = 0;
    std::wstring img_path;
    LogInfo(dial_settings.type);

    if (dial_settings.type == "Dual") {
        img_path = dial_settings.backgroundImageDual;
        headerOffset = 35;
        headerFontSize = 26;
        dataOffset = -2;
        dataFontSize = 48;
        data2Offset = 64;
        data2FontSize = 48;
    } else {
        img_path = (dial_settings.isActive) ? dial_settings.backgroundImageActive : dial_settings.backgroundImageInactive;
        headerOffset = -6;
        headerFontSize = 28;
        dataOffset = 31;
        dataFontSize = 48;
        dial_settings.secDisplayVar = "";
    }

    LogInfo("conversion done");
    std::string base64Image = DrawImage(img_path, dial_settings.header, dial_settings.displayVar, dial_settings.secDisplayVar,
                                        headerOffset, headerFontSize, dataOffset, dataFontSize, data2Offset, data2FontSize);
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
    LogInfo("set image done");
}
