#include "SwitchAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "SimData/SimData.hpp"
// #include "Utils.hpp"

void SwitchAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newEvent = settings.value("toggleEvent", "");
    numPos_ = NlohmannJSONUtils::GetIntByName(settings, "numPos", 2);

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

    // Remove variables if necessary
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
    if (!newFeedback.empty() && newFeedback != feedbackVar_) {
        feedbackVarDef_.name = newFeedback;
        feedbackVarDef_.group = FEEDBACK_VARIABLE;
        varsToRegister.push_back(feedbackVarDef_);
    }

    if (!newEvent.empty() && newEvent != toggleEvent_) {
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
    feedbackVar_ = newFeedback;
    toggleEvent_ = newEvent;

    // Subscribe callbacks
    if (!newFeedback.empty()) {
        feedbackSubId_ = SimManager::Instance().SubscribeToVariable(newFeedback,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
}

void SwitchAction::OnVariableUpdated(const std::string& name, double value) {
    int intValue = static_cast<int>(value);

    auto it = valueToIndex_.find(intValue);
    if (it == valueToIndex_.end()) {
        LogWarn(feedbackVar_ + " Unknown switch position.");
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

    if (!toggleEvent_.empty()) {
        SimManager::Instance().SendEvent(toggleEvent_);
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
    std::vector<SimVarDefinition> vars;
    std::vector<SimEventDefinition> events;
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
    UIManager::Instance().Unregister(this);
}

void SwitchAction::ClearSettings() {
    header_.clear();
    feedbackVar_.clear();
    toggleEvent_.clear();
    positions_.clear();

    curPos_ = 0;
    feedbackSubId_ = 0;
}

void SwitchAction::UpdateImage() {
    std::vector<std::string> labels;
    for (const auto& pos : positions_)
        labels.push_back(pos.label);

    std::string base64Image = DrawSwitchImage(labels, curPos_, header_, SimManager::Instance().IsConnected());
    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
