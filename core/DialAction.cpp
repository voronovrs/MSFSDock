#include "DialAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "SimData/SimData.hpp"

constexpr int DOUBLE_CLICK_MS = 400;

void DialAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");
    skin_ = settings.value("skin", "skin1");

    std::vector<SimVarDefinition> varsToRegister;
    std::vector<SimVarDefinition> varsToDeregister;
    std::vector<SimEventDefinition> eventsToRegister;
    std::vector<SimEventDefinition> eventsToDeregister;

    std::string newDisplay = settings.value("displayVar", "");
    std::string newIncEvent = settings.value("incEvent", "");
    std::string newDecEvent = settings.value("decEvent", "");

    std::string newDisplay2 = settings.value("display2Var", "");
    std::string newInc2Event = settings.value("inc2Event", "");
    std::string newDec2Event = settings.value("dec2Event", "");

    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newEvent = settings.value("toggleEvent", "");

    // Remove variables if necessary
    if (!displayVar_.empty() && displayVar_ != newDisplay) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        varsToDeregister.push_back({ displayVar_, LIVE_VARIABLE });
    }

    if (!display2Var_.empty() && display2Var_ != newDisplay2) {
        if (display2SubId_) {
            SimManager::Instance().UnsubscribeFromVariable(display2Var_, display2SubId_);
        }
        varsToDeregister.push_back({ display2Var_, LIVE_VARIABLE });
    }

    if (!feedbackVar_.empty() && feedbackVar_ != newFeedback) {
        if (feedbackSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(feedbackVar_, feedbackSubId_);
        }
        varsToDeregister.push_back({ feedbackVar_, FEEDBACK_VARIABLE });
    }

    if (!incEvent_.empty() && incEvent_ != newIncEvent) {
        eventsToDeregister.push_back({incEvent_});
    }
    if (!decEvent_.empty() && decEvent_ != newDecEvent) {
        eventsToDeregister.push_back({decEvent_});
    }
    if (!inc2Event_.empty() && inc2Event_ != newInc2Event) {
        eventsToDeregister.push_back({inc2Event_});
    }
    if (!dec2Event_.empty() && dec2Event_ != newDec2Event) {
        eventsToDeregister.push_back({dec2Event_});
    }
    if (!toggleEvent_.empty() && toggleEvent_ != newEvent) {
        eventsToDeregister.push_back({toggleEvent_});
    }

    // Add new variables if necessary
    if (!newDisplay.empty() && newDisplay != displayVar_) {
        displayVarDef_.name = newDisplay;
        displayVarDef_.group = LIVE_VARIABLE;
        varsToRegister.push_back(displayVarDef_);
    }

    if (!newDisplay2.empty() && newDisplay2 != display2Var_) {
        display2VarDef_.name = newDisplay2;
        display2VarDef_.group = LIVE_VARIABLE;
        varsToRegister.push_back(display2VarDef_);
    }

    if (!newFeedback.empty() && newFeedback != feedbackVar_) {
        feedbackVarDef_.name = newFeedback;
        feedbackVarDef_.group = FEEDBACK_VARIABLE;
        varsToRegister.push_back(feedbackVarDef_);
    }

    if (!newIncEvent.empty() && newIncEvent != incEvent_) {
        incEventDef_.name = newIncEvent;
        eventsToRegister.push_back(incEventDef_);
    }
    if (!newDecEvent.empty() && newDecEvent != decEvent_) {
        decEventDef_.name = newDecEvent;
        eventsToRegister.push_back(decEventDef_);
    }
    if (!newInc2Event.empty() && newInc2Event != inc2Event_) {
        inc2EventDef_.name = newInc2Event;
        eventsToRegister.push_back(inc2EventDef_);
    }
    if (!newDec2Event.empty() && newDec2Event != dec2Event_) {
        dec2EventDef_.name = newDec2Event;
        eventsToRegister.push_back(dec2EventDef_);
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
    displayVar_ = newDisplay;
    display2Var_ = newDisplay2;
    feedbackVar_ = newFeedback;
    incEvent_ = newIncEvent;
    decEvent_ = newDecEvent;
    inc2Event_ = newInc2Event;
    dec2Event_ = newDec2Event;
    toggleEvent_ = newEvent;

    // Subscribe callbacks
    if (!newDisplay.empty()) {
        displaySubId_ = SimManager::Instance().SubscribeToVariable(newDisplay,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
    if (!newDisplay2.empty()) {
        display2SubId_ = SimManager::Instance().SubscribeToVariable(newDisplay2,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
    if (!newFeedback.empty()) {
        feedbackSubId_ = SimManager::Instance().SubscribeToVariable(newFeedback,
            [this](const std::string& name, double value) {
                this->OnVariableUpdated(name, value);
            });
    }
}

void DialAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVar_) {
        displayVarDef_.value = value;
    } else if (name == display2Var_) {
        display2VarDef_.value = value;
    } else if (name == feedbackVar_) {
        isActive = (value == 0) ? false : true;
    }
    UpdateImage();
}

void DialAction::DidReceiveSettings(const nlohmann::json& payload) {
    LogInfo("DialAction DidReceiveSettings");
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void DialAction::DialDown(const nlohmann::json& payload) {
    LogInfo("DialAction DialDown");
    if (!SimManager::Instance().IsConnected()) {
        SimManager::Instance().EnsureConnected();
        return;
    }
    if (!isRadio) {
        if (!toggleEvent_.empty()) {
            SimManager::Instance().SendEvent(toggleEvent_);
        }
    }
}

void DialAction::DialUp(const nlohmann::json& payload) {
    LogInfo("DialAction DialUp");

    auto now = std::chrono::steady_clock::now();

    // Double click support
    if (isRadio) {
        if (clickPending_) {
            auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTs_).count();

            if (dt <= DOUBLE_CLICK_MS) {
                LogInfo("DialAction Double click");

                clickPending_ = false;

                if (!toggleEvent_.empty()) {
                    SimManager::Instance().SendEvent(toggleEvent_);
                }

                UpdateImage();
                return;
            }
        }

        // First click
        active_radio_part ^= 1;
        UpdateImage();
        clickPending_ = true;
        lastClickTs_ = now;
        return;
    }

    if (isDual && !isRadio) {
        active_dial ^= 1;
        UpdateImage();
    }
}

void DialAction::RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed){
    LogInfo("DialAction clockwise");
    if ((isDual && active_dial == 1) || (isRadio && active_radio_part == 1)) {
        if (!inc2Event_.empty()) {
            SimManager::Instance().SendEvent(inc2Event_);
        }
    } else {
        if (!incEvent_.empty()) {
            SimManager::Instance().SendEvent(incEvent_);
        }
    }
}

void DialAction::RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed){
    LogInfo("DialAction Counterclockwise");
    if ((isDual && active_dial == 1) || (isRadio && active_radio_part == 1)) {
        if (!dec2Event_.empty()) {
            SimManager::Instance().SendEvent(dec2Event_);
        }
    } else {
        if (!decEvent_.empty()) {
            SimManager::Instance().SendEvent(decEvent_);
        }
    }
}

void DialAction::SendToPI(const nlohmann::json& payload) {
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

void DialAction::WillAppear(const nlohmann::json& payload) {
    LogInfo("DialAction WillAppear");
    UIManager::Instance().Register(this);
    UpdateVariablesAndEvents(payload);
    UpdateImage();
}

void DialAction::WillDisappear(const nlohmann::json& /*payload*/) {
    // Deregister only the vars coming from this action
    LogInfo("DialAction WillDisappear");
    std::vector<SimVarDefinition> vars;
    std::vector<SimEventDefinition> events;
    if (!displayVar_.empty()) {
        if (displaySubId_) {
            SimManager::Instance().UnsubscribeFromVariable(displayVar_, displaySubId_);
        }
        vars.push_back({displayVar_, LIVE_VARIABLE});
    }
    if (!display2Var_.empty()) {
        if (display2SubId_) {
            SimManager::Instance().UnsubscribeFromVariable(display2Var_, display2SubId_);
        }
        vars.push_back({display2Var_, LIVE_VARIABLE});
    }
    if (!feedbackVar_.empty()) {
        if (feedbackSubId_) {
            SimManager::Instance().UnsubscribeFromVariable(feedbackVar_, feedbackSubId_);
        }
        vars.push_back({feedbackVar_, FEEDBACK_VARIABLE});
    }
    if (!vars.empty()) {
        SimManager::Instance().RemoveSimVars(vars);
    }

    if (!incEvent_.empty()) {
        events.push_back({incEvent_});
    }
    if (!decEvent_.empty()) {
        events.push_back({decEvent_});
    }
    if (!inc2Event_.empty()) {
        events.push_back({inc2Event_});
    }
    if (!dec2Event_.empty()) {
        events.push_back({dec2Event_});
    }
    if (!toggleEvent_.empty()) {
        events.push_back({toggleEvent_});
    }
    if (!events.empty())
        SimManager::Instance().RemoveSimEvents(events);

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void DialAction::ClearSettings() {
    header_.clear();
    displayVar_.clear();
    display2Var_.clear();
    feedbackVar_.clear();
    incEvent_.clear();
    decEvent_.clear();
    inc2Event_.clear();
    dec2Event_.clear();
    toggleEvent_.clear();

    displaySubId_ = 0;
    feedbackSubId_ = 0;
}

void DialAction::UpdateImage() {
    int headerOffset, headerFontSize, dataOffset, dataFontSize, data2Offset;
    std::wstring backgroundImageInactive, backgroundImageActive;
    Gdiplus::Color header_color, active_data_color, inactive_data_color, data1_color, data2_color;

    if (skin_ == "skin1") {
        backgroundImageInactive = (isDual) ? ((active_dial == 0) ? b_Dual_1 : b_Dual_2) : b_Inactive;
        backgroundImageActive = b_Active;
        header_color = COLOR_OFF_WHITE;
        active_data_color = COLOR_WHITE;
        inactive_data_color = COLOR_GRAY;
    } else {
        backgroundImageInactive = (isDual) ? ((active_dial == 0) ? ab_Dual_1 : ab_Dual_2) : ab_Inactive;
        backgroundImageActive = ab_Active;
        header_color = COLOR_ORANGE;
        active_data_color = COLOR_BRIGHT_ORANGE;
        inactive_data_color = COLOR_ORANGE;
    }

    if (isDual) {
        headerOffset = 36;
        headerFontSize = 26;
        dataOffset = 0;
        dataFontSize = 46;
        data2Offset = 66;
        data1_color = (active_dial == 0) ? active_data_color : inactive_data_color;
        data2_color = (active_dial == 1) ? active_data_color : inactive_data_color;
    } else {
        headerOffset = -6;
        headerFontSize = 28;
        dataOffset = 32;
        dataFontSize = 46;
        data2Offset = 170;
        data1_color = active_data_color;
        data2_color = active_data_color;
    }

    std::string base64Image;
    std::wstring img_path = (isActive) ? backgroundImageActive : backgroundImageInactive;

    if (!isRadio) {
        std::string val = (displayVar_.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));
        std::string val2 = (display2Var_.empty()) ? "" : std::to_string(static_cast<int>(display2VarDef_.value));
        base64Image = DrawDialImage(img_path, header_, header_color,
                                    val, data1_color,
                                    val2, data2_color,
                                    headerOffset, headerFontSize,
                                    dataOffset, dataFontSize,
                                    data2Offset, dataFontSize, SimManager::Instance().IsConnected());
    } else {
        double v = displayVar_.empty() ? 0.0 : displayVarDef_.value;
        int total = static_cast<int>(std::round(v * 1000.0));
        int int_val  = total / 1000;
        int frac_val = total % 1000;
        double stdb_v = display2Var_.empty() ? 0.0 : display2VarDef_.value;
        total = static_cast<int>(std::round(stdb_v * 1000.0));
        int stdb_int_val  = total / 1000;
        int stdb_frac_val = total % 1000;
        data1_color = (active_radio_part == 0) ? active_data_color : inactive_data_color;
        data2_color = (active_radio_part == 1) ? active_data_color : inactive_data_color;
        base64Image = DrawRadioImage(img_path, header_, header_color,
                                    int_val, frac_val, stdb_int_val, stdb_frac_val,
                                    active_data_color, data1_color, data2_color,
                                    headerOffset, headerFontSize,
                                    dataOffset, dataFontSize,
                                    data2Offset, dataFontSize, SimManager::Instance().IsConnected());
    }

    SetImage(base64Image, kESDSDKTarget_HardwareAndSoftware, -1);
}
