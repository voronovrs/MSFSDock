#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include "StreamDockCPPSDK/StreamDockSDK/HSDAction.h"
#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "SimData/SimData.hpp"
#include "SimData/SimVar.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/UIManager.hpp"

namespace BaseActionEvents
{
    using Actions = std::array<uint32_t, 2>;

    inline constexpr Actions GENERIC = {0, 0};
    inline constexpr Actions PMDG_CLICK = {MOUSE_FLAG_LEFTSINGLE, MOUSE_FLAG_LEFTRELEASE};
    inline constexpr Actions PMDG_SCROLL_UP = {MOUSE_FLAG_WHEEL_UP, 0};
    inline constexpr Actions PMDG_SCROLL_DOWN = {MOUSE_FLAG_WHEEL_DOWN, 0};
}

// Variable procession structure
struct VarBinding {
    SimVarDefinition* def;                  // Pointer to variable def
    std::string  next;                      // New variable name
    DEFINITIONS  group;                     // Variable group
    SubscriptionId* subId;                  // subscription ID pointer
};

// Event procession structure
struct EventBinding {
    SimEventDefinition* def;                // Pointer to event def
    std::string  next;                      // New event name
    EVENT_TYPES type;                       // Event type
    std::array<uint32_t, 2> actions{};      // Event actions
};

class BaseAction : public HSDAction, public IUIUpdatable {
public:
    BaseAction(HSDConnectionManager* hsd_connection,
               const std::string& action,
               const std::string& context);
    virtual ~BaseAction();

protected:
    std::vector<VarBinding> varBindings_;
    std::vector<EventBinding> eventBindings_;

    virtual void OnVariableUpdated(const std::string& name, double value);

    // Public interface for derived classes
    void ApplyBindings();
    void UnregisterAll();
    void CleanUp();

private:
    std::function<void(const std::string&, double)> varCallback_;

    // Helper methods
    static void FillEvent(SimEventDefinition& e, const std::string& name,
                          EVENT_TYPES type, const std::array<uint32_t, 2>& actions);

    void DiffVar(VarBinding& v, std::vector<SimVarDefinition>& toAdd, std::vector<SimVarDefinition>& toRemove);
    void SubscribeVar(VarBinding& v, const std::function<void(const std::string&, double)>& callback);
    void DiffEvent(EventBinding& e, std::vector<SimEventDefinition>& toAdd, std::vector<SimEventDefinition>& toRemove);
    void ApplyChanges(std::vector<SimVarDefinition>& varsToAdd,
                      std::vector<SimVarDefinition>& varsToRemove,
                      std::vector<SimEventDefinition>& eventsToAdd,
                      std::vector<SimEventDefinition>& eventsToRemove);
};
