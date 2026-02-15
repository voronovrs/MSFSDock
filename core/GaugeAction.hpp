#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BaseAction.hpp"
#include "ui/GDIPlusManager.hpp"

enum DataFormat {
    DATA_FMT_INT = 0,
    DATA_FMT_PERCENT,
};

enum GaugeSkin {
    GAUGE_SKIN_CIRCULAR = 0,
    GAUGE_SKIN_VERTICAL,
};

class GaugeAction : public BaseAction {
public:

    GaugeAction(HSDConnectionManager* hsd_connection,
                const std::string& action,
                const std::string& context)
        : BaseAction(hsd_connection, action, context)
    {}

    virtual void SendToPI(const nlohmann::json& payload) override;
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
    GaugeSkin skinType_ = GAUGE_SKIN_CIRCULAR;
    int maxVal_;
    int minVal_;
    bool fill_ = false;

    std::string scaleColor_ = "#ffff00";
    std::string indicatorColor_ = "#8b0000";
    std::string bgColor_ = "#141414";

    // Vertical gauge specific
    std::vector<ScaleMarker> scaleMarkers_;

    SimVarDefinition displayVarDef_;

    SubscriptionId displaySubId_ = 0;
};
