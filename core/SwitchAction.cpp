#include "SwitchAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "Utils.hpp"

namespace EVT = BaseActionEvents;

void SwitchAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");

    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newEvent = settings.value("toggleEvent", "");
    numPos_ = getIntFromJson(settings, "numPos", 2);

    positions_.clear();
    valueToIndex_.clear();

    int idx = 0;

    for (int i = 0; i < numPos_; ++i) {
        const std::string valueKey = "pos" + std::to_string(i) + "_value";
        const std::string labelKey = "pos" + std::to_string(i) + "_label";

        std::string valueStr = std::to_string(i);
        std::string labelStr = "POS " + std::to_string(i);

        if (settings.contains(valueKey) && settings[valueKey].is_string()) {
            std::string tmp = settings[valueKey].get<std::string>();
            if (!tmp.empty()) {
                valueStr = tmp;
            }
        }

        if (settings.contains(labelKey) && settings[labelKey].is_string()) {
            std::string tmp = settings[labelKey].get<std::string>();
            if (!tmp.empty()) {
                labelStr = tmp;
            }
        }

        int simValue = 0;
        try {
            simValue = std::stoi(valueStr);
        } catch (...) {
            simValue = i; // fallback
        }

        positions_.push_back({ simValue, labelStr });
        valueToIndex_[simValue] = idx++;
    }

    curPos_ = 0;

    varBindings_ = {
        {&feedbackVarDef_, newFeedback, FEEDBACK_VARIABLE, &feedbackSubId_},
    };

    eventBindings_ = {
        { &toggleEventDef_, newEvent, EVENT_GENERIC, EVT::GENERIC},
    };

    ApplyBindings();
}

void SwitchAction::OnVariableUpdated(const std::string& name, double value) {
    int intValue = static_cast<int>(value);

    auto it = valueToIndex_.find(intValue);
    if (it == valueToIndex_.end()) {
        LogWarn(feedbackVarDef_.name + " Unknown switch position.");
        return;
    }

    curPos_ = it->second;
    UpdateImage();

}

void SwitchAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("SwitchAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void SwitchAction::KeyDown(const nlohmann::json& payload) {
    LogInfo("SwitchAction KeyDown");
    if (!SimManager::Instance().IsConnected()) {
        SimManager::Instance().EnsureConnected();
        return;
    }

    if (!toggleEventDef_.name.empty()) {
        SimManager::Instance().SendEvent(toggleEventDef_.name);
    }
}

void SwitchAction::KeyUp(const nlohmann::json& /*payload*/) {
    // LogInfo("SwitchAction KeyUp");
    // not used for now
}

void SwitchAction::SendToPI(const nlohmann::json& payload) {
    nlohmann::json out_payload;
    out_payload["type"] = "evt_var_list";
    out_payload["common_events"] = nlohmann::json::array();
    out_payload["common_variables"] = nlohmann::json::array();

    for (const auto& evt : GetKnownVariables()) {
        out_payload["common_variables"].push_back(evt);
    }

    for (const auto& evt : GetKnownEvents()) {
        out_payload["common_events"].push_back(evt);
    }

    SendToPropertyInspector(out_payload);
}

void SwitchAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("SwitchAction WillAppear");
    UIManager::Instance().Register(this);
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void SwitchAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("SwitchAction WillDisappear");
    UnregisterAll();

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void SwitchAction::ClearSettings() {
    header_.clear();
    CleanUp();
    curPos_ = 0;
}

void SwitchAction::UpdateImage() {
    std::vector<std::string> labels;
    for (const auto& pos : positions_)
        labels.push_back(pos.label);

    std::string base64Image = DrawSwitchImage(labels, curPos_, header_, SimManager::Instance().IsConnected());
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
