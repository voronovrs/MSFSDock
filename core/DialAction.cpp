#include <algorithm>
#include "DialAction.hpp"
#include "plugin/Logger.hpp"
#include "ui/GDIPlusManager.hpp"
#include "Utils.hpp"

constexpr int DOUBLE_CLICK_MS = 400;
namespace EVT = BaseActionEvents;

void DialAction::UpdateVariablesAndEvents(const nlohmann::json& payload) {
    if (!payload.contains("settings")) return;
    const auto& settings = payload["settings"];

    header_ = settings.value("header", "");
    skin_ = settings.value("skin", "skin1");
    accelMultiplier_ = getIntFromJson(settings, "accelMultiplier", 3);

    std::string newDisplay = settings.value("displayVar", "");
    std::string newIncEvent = settings.value("incEvent", "");
    std::string newDecEvent = settings.value("decEvent", "");

    std::string newDisplay2 = settings.value("display2Var", "");
    std::string newInc2Event = settings.value("inc2Event", "");
    std::string newDec2Event = settings.value("dec2Event", "");

    std::string newFeedback = settings.value("feedbackVar", "");
    std::string newEvent = settings.value("toggleEvent", "");


    varBindings_ = {
        {&displayVarDef_, newDisplay, (isPmdg) ? PMDG_VARIABLE : LIVE_VARIABLE, &displaySubId_},
        {&display2VarDef_, newDisplay2, (isPmdg) ? PMDG_VARIABLE : LIVE_VARIABLE, &display2SubId_},
        {&feedbackVarDef_, newFeedback, (isPmdg) ? PMDG_VARIABLE : LIVE_VARIABLE, &feedbackSubId_},
    };

    eventBindings_ = {
        { &toggleEventDef_, newEvent, (isPmdg) ? EVENT_PMDG : EVENT_GENERIC, (isPmdg) ? EVT::PMDG_CLICK : EVT::GENERIC },
        { &incEventDef_, newIncEvent, (isPmdg) ? EVENT_PMDG : EVENT_GENERIC, (isPmdg) ? EVT::PMDG_SCROLL_UP : EVT::GENERIC },
        { &decEventDef_, newDecEvent, (isPmdg) ? EVENT_PMDG : EVENT_GENERIC, (isPmdg) ? EVT::PMDG_SCROLL_DOWN : EVT::GENERIC },
        { &inc2EventDef_, newInc2Event, (isPmdg) ? EVENT_PMDG : EVENT_GENERIC, (isPmdg) ? EVT::PMDG_SCROLL_UP : EVT::GENERIC },
        { &dec2EventDef_, newDec2Event, (isPmdg) ? EVENT_PMDG : EVENT_GENERIC, (isPmdg) ? EVT::PMDG_SCROLL_DOWN : EVT::GENERIC },
    };

    ApplyBindings();

}

void DialAction::OnVariableUpdated(const std::string& name, double value) {
    if (name == displayVarDef_.name) {
        displayVarDef_.value = value;
    } else if (name == display2VarDef_.name) {
        display2VarDef_.value = value;
    } else if (name == feedbackVarDef_.name) {
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
        if (!toggleEventDef_.name.empty()) {
            SimManager::Instance().SendEvent(toggleEventDef_.uniqueName);
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

                if (!toggleEventDef_.name.empty()) {
                    SimManager::Instance().SendEvent(toggleEventDef_.uniqueName);
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

uint8_t DialAction::GetEventsCount() {
    if (!accelMultiplier_)
        return 1;

    constexpr double accelThreshold = 120.0;
    constexpr double minDelta = 15.0;
    constexpr double invRange = 1.0 / (accelThreshold - minDelta);

    auto now = std::chrono::steady_clock::now();

    if (lastRotateTs_.time_since_epoch().count() == 0) {
        lastRotateTs_ = now;
        return 1;
    }

    double delta = std::chrono::duration<double, std::milli>(now - lastRotateTs_).count();
    lastRotateTs_ = now;

    double t = std::clamp((accelThreshold - delta) * invRange, 0.0, 1.0);

    double multiplier = 1.0 + ((double)accelMultiplier_ - 1.0) * t * t * t;

    return static_cast<uint8_t>(std::round(multiplier));
}

void DialAction::RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) {
    LogInfo("DialAction clockwise");
    if ((isDual && active_dial == 1) || (isRadio && active_radio_part == 1)) {
        if (!inc2EventDef_.name.empty()) {
            SimManager::Instance().SendEvent(inc2EventDef_.uniqueName, GetEventsCount());
        }
    } else {
        if (!incEventDef_.name.empty()) {
            SimManager::Instance().SendEvent(incEventDef_.uniqueName, GetEventsCount());
        }
    }
}

void DialAction::RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) {
    LogInfo("DialAction Counterclockwise");
    if ((isDual && active_dial == 1) || (isRadio && active_radio_part == 1)) {
        if (!dec2EventDef_.name.empty()) {
            SimManager::Instance().SendEvent(dec2EventDef_.uniqueName, GetEventsCount());
        }
    } else {
        if (!decEventDef_.name.empty()) {
            SimManager::Instance().SendEvent(decEventDef_.uniqueName, GetEventsCount());
        }
    }
}

void DialAction::SendToPI(const nlohmann::json& payload) {
    nlohmann::json out_payload = BuildCommonPayloadJson(isPmdg);

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

    UnregisterAll();

    ClearSettings();
    UIManager::Instance().Unregister(this);
}

void DialAction::ClearSettings() {
    header_.clear();
    CleanUp();
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
        headerOffset = 42;
        headerFontSize = 28;
        dataOffset = 0;
        dataFontSize = 46;
        data2Offset = 66;
        data1_color = (active_dial == 0) ? active_data_color : inactive_data_color;
        data2_color = (active_dial == 1) ? active_data_color : inactive_data_color;
    } else {
        headerOffset = 0;
        headerFontSize = 32;
        dataOffset = 32;
        dataFontSize = 46;
        data2Offset = 170;
        data1_color = active_data_color;
        data2_color = active_data_color;
    }

    std::string base64Image;
    std::wstring img_path = (isActive) ? backgroundImageActive : backgroundImageInactive;

    if (!isRadio) {
        std::string val = (displayVarDef_.name.empty()) ? "" : std::to_string(static_cast<int>(displayVarDef_.value));
        std::string val2 = (display2VarDef_.name.empty()) ? "" : std::to_string(static_cast<int>(display2VarDef_.value));
        base64Image = DrawDialImage(img_path, header_, header_color,
                                    val, data1_color,
                                    val2, data2_color,
                                    headerOffset, headerFontSize,
                                    dataOffset, dataFontSize,
                                    data2Offset, dataFontSize, SimManager::Instance().IsConnected());
    } else {
        double v = displayVarDef_.name.empty() ? 0.0 : displayVarDef_.value;
        int total = static_cast<int>(std::round(v * 1000.0));
        int int_val  = total / 1000;
        int frac_val = total % 1000;
        double stdb_v = display2VarDef_.name.empty() ? 0.0 : display2VarDef_.value;
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
