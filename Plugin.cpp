
#include "Logger.h"
#include "Plugin.h"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"


std::shared_ptr<HSDAction> MSFSDockPlugin::GetOrCreateAction(const std::string& action, const std::string& context)
{
    auto it = mActions.find(context);
    if (it != mActions.end()) {
        return it->second;
    }

    if (action == "com.rvoronov.msfsDock.generic.toggle") {
        auto impl = std::make_shared<ToggleAction>(
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
            context
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
