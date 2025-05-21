#pragma once

#include <string>
#include <Windows.h>
#include "StreamDockSDK/HSDAction.h"

struct PluginSettings {
    const std::wstring backgroundImageInactive = L"images/button.png";
    const std::wstring backgroundImageActive = L"images/button_active.png";
    std::string header;
    std::string toggleEvent;
    std::string feedbackVar;
    std::string displayVar;
    bool isActive = false;

    void FromJson(const nlohmann::json& json) {
        if (json.contains("header")) header = json["header"].get<std::string>();
        if (json.contains("toggleEvent")) toggleEvent = json["toggleEvent"].get<std::string>();
        if (json.contains("feedbackVar")) feedbackVar = json["feedbackVar"].get<std::string>();
        if (json.contains("displayVar")) displayVar = json["displayVar"].get<std::string>();
        // Add other fields if needed
    }
};

class ToggleAction : public HSDAction
{
    using HSDAction::HSDAction;

    PluginSettings toggle_settings;

    virtual void DidReceiveSettings(const nlohmann::json& payload);
    virtual void KeyDown(const nlohmann::json& payload);
    // virtual void DialDown(const nlohmann::json& payload);
    virtual void KeyUp(const nlohmann::json& payload);
    virtual void SendToPlugin(const nlohmann::json& payload);
    virtual void WillAppear(const nlohmann::json& payload);
    virtual void WillDisappear(const nlohmann::json& payload);
    virtual void UpdateImage();
};
