#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimVar.hpp"
#include "ui/GDIPlusManager.hpp"

#ifdef SIM_MOCK
#include "sim/SimManagerMock.hpp"
#else
#include "sim/SimManager.hpp"
#endif

class GaugeAction : public HSDAction {
public:
    using HSDAction::HSDAction;

    virtual void DidReceiveSettings(const nlohmann::json& payload) override;
    virtual void KeyDown(const nlohmann::json& payload) override;
    virtual void KeyUp(const nlohmann::json& payload) override;
    virtual void WillAppear(const nlohmann::json& payload) override;
    virtual void WillDisappear(const nlohmann::json& payload) override;
    void OnVariableUpdated(const std::string& name, double value);
    void UpdateImage();

    std::string displayValue_;

private:
    void UpdateVariablesAndEvents(const nlohmann::json& payload);
    void ClearSettings();

    // parsed settings
    std::string header_;
    std::string displayVar_;
    int maxVal_;
    int minVal_;
    bool fill_ = false;
    Gdiplus::Color scaleColor_ = COLOR_YELLOW;
    Gdiplus::Color indicatorColor_ = COLOR_RED;
    Gdiplus::Color bgColor_ = COLOR_NEAR_BLACK;

    SimVarDefinition displayVarDef_;

    SubscriptionId displaySubId_ = 0;
};
