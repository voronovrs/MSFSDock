#pragma once

#include <string>
#include <Windows.h>
#include "StreamDockSDK/HSDAction.h"

struct DialPluginSettings {
    const std::wstring backgroundImageInactive = L"images/dial.png";
    const std::wstring backgroundImageActive = L"images/dial_active.png";
    const std::wstring backgroundImageDual = L"images/dualdial.png";
    std::string type;
    std::string header;
    std::string incEvent;
    std::string decEvent;
    std::string pushEvent;
    std::string displayVar;
    std::string secDisplayVar;
    std::string secIncEvent;
    std::string secDecEvent;
    std::string feedbackVar;
    bool isActive = false;

    void FromJson(const nlohmann::json& json) {
        if (json.contains("type")) type = json["type"].get<std::string>();
        if (json.contains("header")) header = json["header"].get<std::string>();
        if (json.contains("incEvent")) incEvent = json["incEvent"].get<std::string>();
        if (json.contains("decEvent")) decEvent = json["decEvent"].get<std::string>();
        if (json.contains("pushEvent")) pushEvent = json["pushEvent"].get<std::string>();
        if (json.contains("displayVar")) displayVar = json["displayVar"].get<std::string>();
        if (json.contains("secDisplayVar")) secDisplayVar = json["secDisplayVar"].get<std::string>();
        if (json.contains("secIncEvent")) secIncEvent = json["secIncEvent"].get<std::string>();
        if (json.contains("secDecEvent")) secDecEvent = json["secDecEvent"].get<std::string>();
        if (json.contains("feedbackVar")) feedbackVar = json["feedbackVar"].get<std::string>();
        // Add other fields if needed
    }
};

class DialAction : public HSDAction
{
    using HSDAction::HSDAction;

    DialPluginSettings dial_settings;

    virtual void DidReceiveSettings(const nlohmann::json& payload);
    virtual void DialUp(const nlohmann::json& payload);
    virtual void DialDown(const nlohmann::json& payload);
    // virtual void RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed);
    // virtual void RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed);
    virtual void SendToPlugin(const nlohmann::json& payload);
    virtual void WillAppear(const nlohmann::json& payload);
    virtual void WillDisappear(const nlohmann::json& payload);
    virtual void UpdateImage();
};
