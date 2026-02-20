#include "GaugeAction.hpp"
#include "plugin/Logger.hpp"
#include "Utils.hpp"

namespace EVT = BaseActionEvents;

void GaugeAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings"))
        return;

    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");

    minVal_ = getIntFromJson(settings, "minVal", 0);
    maxVal_ = getIntFromJson(settings, "maxVal", 10000);

    fill_ = (settings.value("style", "") == "fill") ? true : false;
    dataFormat = (settings.value("dataFormat", "") == "percent") ? DATA_FMT_PERCENT : DATA_FMT_INT;
    skinType_ = (settings.value("skin", "") == "vertical") ? GAUGE_SKIN_VERTICAL : GAUGE_SKIN_CIRCULAR;
    scaleColor_ = settings.value("scaleColor", "#ffff00");
    indicatorColor_ = settings.value("indicatorColor", "#8b0000");
    bgColor_ = settings.value("bgColor", "#141414");

    // Vertical gauge specific settings
    scaleMarkers_.clear();
    if (settings.contains("scaleMarkers") && settings["scaleMarkers"].is_array()) {
        for (const auto& m : settings["scaleMarkers"]) {
            ScaleMarker sm;
            sm.position = m.value("position", 0);
            sm.color = m.value("color", "#ffffff");
            scaleMarkers_.push_back(sm);
        }
    }

    std::string newDisplay = settings.value("displayVar", "");

    varBindings_ = {
        {&displayVarDef_, newDisplay, LIVE_VARIABLE, &displaySubId_},
    };

    ApplyBindings();
}

void GaugeAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVarDef_.name) {
        displayVarDef_.value = value;
    }
    UpdateImage();
}

void GaugeAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("GaugeAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void GaugeAction::KeyDown(const nlohmann::json& payload) {
    LogInfo("GaugeAction KeyDown");
    if (!SimManager::Instance().IsConnected()) {
        SimManager::Instance().EnsureConnected();
    }
}

void GaugeAction::KeyUp(const nlohmann::json& /*payload*/) {
    // not used for now
}

void GaugeAction::SendToPI(const nlohmann::json& payload) {
    nlohmann::json out_payload = BuildCommonPayloadJson();

    SendToPropertyInspector(out_payload);
}

void GaugeAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("GaugeAction WillAppear");
    UIManager::Instance().Register(this);
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void GaugeAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("GaugeAction WillDisappear");
    UnregisterAll();

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void GaugeAction::ClearSettings() {
    header_.clear();
    CleanUp();
}

void GaugeAction::UpdateImage() {
    Gdiplus::Color header_color, data_color;
    header_color = COLOR_OFF_WHITE;
    data_color = COLOR_WHITE;

    std::string data;
    double value;

    if (skinType_ == GAUGE_SKIN_VERTICAL) {
        // Vertical gauge: percent is calculated relative to configured min/max range
        value = displayVarDef_.value;
        switch (dataFormat) {
            case DATA_FMT_INT:
                data = std::to_string(static_cast<int>(value));
                break;
            case DATA_FMT_PERCENT: {
                double range = static_cast<double>(maxVal_) - static_cast<double>(minVal_);
                int percent = 0;
                if (range != 0.0) {
                    percent = static_cast<int>(std::round(((value - minVal_) / range) * 100.0));
                }
                data = std::to_string(percent) + "%";
                break;
            }
            default:
                data = "0";
                break;
        }

        int headerOffset = 57, headerFontSize = 12, dataOffset = 0, dataFontSize = 16;
        std::string base64Image = DrawVerticalGaugeImage(header_, header_color, value, data, data_color,
            headerOffset, headerFontSize, dataOffset, dataFontSize, minVal_, maxVal_, fill_,
            scaleColor_, indicatorColor_, bgColor_, SimManager::Instance().IsConnected(),
            scaleMarkers_);
        SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
    } else {
        // Circular gauge (original behavior)
        switch (dataFormat) {
            case DATA_FMT_INT:
                data = std::to_string(static_cast<int>(displayVarDef_.value));
                value = displayVarDef_.value;
                break;
            case DATA_FMT_PERCENT: {
                int percent = static_cast<int>(std::round(displayVarDef_.value * 100.0));
                data = std::to_string(percent) + "%";
                value = std::round(displayVarDef_.value * 100.0);
                break;
            }
            default:
                data = "0";
                value = 0.0;
                break;
        }

        int headerOffset = 44, headerFontSize = 14, dataOffset = 22, dataFontSize = 20;
        std::string base64Image = DrawGaugeImage(header_, header_color, value, data, data_color,
            headerOffset, headerFontSize, dataOffset, dataFontSize, minVal_, maxVal_, fill_,
            scaleColor_, indicatorColor_, bgColor_, SimManager::Instance().IsConnected());
        SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
    }
}
