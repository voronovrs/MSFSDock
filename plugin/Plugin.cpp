
#include "Logger.hpp"
#include "Plugin.hpp"
#include "core/ButtonAction.hpp"
#include "core/DialAction.hpp"
#include "core/GaugeAction.hpp"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"


std::shared_ptr<HSDAction> MSFSDockPlugin::GetOrCreateAction(const std::string& action, const std::string& context)
{
    auto it = mActions.find(context);
    if (it != mActions.end()) {
        return it->second;
    }

    if (action == "com.rvoronov.msfsDock.generic.toggle") {
        auto impl = std::make_shared<ButtonAction>(
            mConnectionManager,
            action,
            context
            );
        mActions.emplace(context, impl);
        return impl;
    } else if (action == "com.rvoronov.msfsDock.generic.gauge") {
        auto impl = std::make_shared<GaugeAction>(
            mConnectionManager,
            action,
            context
            );
        mActions.emplace(context, impl);
        return impl;
    } else if (action == "com.rvoronov.msfsDock.generic.dial") {
        auto impl = std::make_shared<DialAction>(
            mConnectionManager,
            action,
            context,
            false,
            false
            );
        mActions.emplace(context, impl);
        return impl;
    } else if (action == "com.rvoronov.msfsDock.generic.dualdial") {
        auto impl = std::make_shared<DialAction>(
            mConnectionManager,
            action,
            context,
            true,
            false
            );
        mActions.emplace(context, impl);
        return impl;
    } else if (action == "com.rvoronov.msfsDock.generic.radio") {
        auto impl = std::make_shared<DialAction>(
            mConnectionManager,
            action,
            context,
            true,
            true
            );
        mActions.emplace(context, impl);
        return impl;
    }

    LogError("Asked to get or create unknown action: " + action);
    return nullptr;
}

void MSFSDockPlugin::DidReceiveGlobalSettings(const nlohmann::json& payload)
{
    LogInfo("Received global settings");
    LogInfo("Global settings: " + payload.dump());

    // Do plugin-wide stuff here, e.g. reconnect to application being
    // controlled

    for (const auto& action : mActions) {
        // ... then pass it on to each action, e.g.:
        // action->setApplicationConnection(myConnectionObject);
    }
}
