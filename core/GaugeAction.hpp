#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimVar.hpp"
#include "ui/GDIPlusManager.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/UIManager.hpp"

enum DataFormat {
    DATA_FMT_INT = 0,
    DATA_FMT_PERCENT,
};

class GaugeAction : public HSDAction, public IUIUpdatable {
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
    DataFormat dataFormat = DATA_FMT_INT;
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
