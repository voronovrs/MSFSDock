#include "GaugeAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"

void GetColor(Gdiplus::Color& color, const std::string& cfg_val) {
    static const std::unordered_map<std::string, Gdiplus::Color> colorMap = {
        { "white",          COLOR_WHITE },
        { "orange",         COLOR_ORANGE },
        { "gray",           COLOR_GRAY },
        { "yellow",         COLOR_YELLOW },
        { "red",            COLOR_RED },
        { "green",          COLOR_GREEN },
        { "darkGreen",      COLOR_DARK_GREEN },
        { "cyan",           COLOR_CYAN },
        { "blue",           COLOR_BLUE },
        { "darkBlue",       COLOR_DARK_BLUE },
        { "black",          COLOR_NEAR_BLACK },
    };

    auto it = colorMap.find(cfg_val);
    if (it != colorMap.end())
        color = it->second;
}

void GaugeAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings"))
        return;

    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");
    minVal_ = settings.value("minVal", 0);
    maxVal_ = settings.value("maxVal", 10000);
    fill_ = (settings.value("style", "") == "fill") ? true : false;
    GetColor(scaleColor_, settings.value("scaleColor", std::string{}));
    GetColor(indicatorColor_, settings.value("indicatorColor", std::string{}));
    GetColor(bgColor_, settings.value("bgColor", std::string{}));

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;

    std::string newDisplay = settings.value("displayVar", "");

    // Remove variables if necessary
    if (!displayVar_.empty() && displayVar_ != newDisplay) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        varsToDeregister.push_back({ displayVar_, LIVE_VARIABLE });
    }

    // Add new variables if necessary
    if (!newDisplay.empty() && newDisplay != displayVar_) {
        displayVarDef_.name = newDisplay;
        displayVarDef_.group = LIVE_VARIABLE;
        varsToRegister.push_back(displayVarDef_);
    }

    // Call add/remove
    if (!varsToDeregister.empty())
        SimManager::Instance().RemoveSimVars(varsToDeregister);

    if (!varsToRegister.empty())
        SimManager::Instance().AddSimVars(varsToRegister);

    // Save new values
    displayVar_ = newDisplay;

    // Subscribe callbacks
    if (!newDisplay.empty()) {
        displaySubId_ = SimManager::Instance().SubscribeToVariable(newDisplay,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
}

void GaugeAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVar_) {
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
    // not used for now
}

void GaugeAction::KeyUp(const nlohmann::json& /*payload*/) {
    // not used for now
}

void GaugeAction::WillAppear(const nlohmann::json& payload) {
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void GaugeAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("GaugeAction WillDisappear");
    std::vector<SimVarDefinition> vars;
    if (!displayVar_.empty()) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        vars.push_back({displayVar_, LIVE_VARIABLE});
    }
    if (!vars.empty()) {
        SimManager::Instance().RemoveSimVars(vars);
    }

    ClearSettings();
}

void GaugeAction::ClearSettings() {
    header_.clear();
    displayVar_.clear();

    displaySubId_ = 0;
}

void GaugeAction::UpdateImage() {
    int headerOffset = 44, headerFontSize = 14, dataOffset = 20, dataFontSize = 20;
    Gdiplus::Color header_color, data_color;
    header_color = COLOR_OFF_WHITE;
    data_color = COLOR_WHITE;

    std::string base64Image = DrawGaugeImage(header_, header_color, displayVarDef_.value, data_color,
        headerOffset, headerFontSize, dataOffset, dataFontSize, minVal_, maxVal_, fill_,
        scaleColor_, indicatorColor_, bgColor_);
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
