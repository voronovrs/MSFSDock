#include "plugin/Logger.hpp"
#include "SimData.hpp"
#include "KnownVariables.hpp"
#include "PMDG_NG3_SDK.h"


// COMMON EVENT NAMES FOR UI
#define X(name) #name,

static const std::vector<std::string> KNOWN_EVENTS = {
#include "common_events.inc"
};

#undef X

const std::vector<std::string>& GetKnownEvents() {
	return KNOWN_EVENTS;
}

// PMDG EVENT ID LOOKUP
#define X(name) { #name, name },

static const std::unordered_map<std::string, int> PMDG_EVENT_MAP = {
#include "pmdg_events.inc"
};

#undef X

// PMDG EVENT NAMES FOR UI
#define X(name) #name,

static const std::vector<std::string> PMDG_EVENT_NAMES = {
#include "pmdg_events.inc"
};

#undef X

const std::vector<std::string>& GetPmdgEvents() {
    return PMDG_EVENT_NAMES;
}

uint32_t GetPmdgEventID(const std::string& name) {
    auto it = PMDG_EVENT_MAP.find(name);

    if (it == PMDG_EVENT_MAP.end()) {
        LogError("Can't find ID for PMDG event " + name);
        return THIRD_PARTY_EVENT_ID_MIN;
    }

    return it->second;
}

std::vector<std::string> GetKnownVariables() {
    std::vector<std::string> vars;
    vars.reserve(KnownVariables::kMap.size());

    for (const auto& [name, _] : KnownVariables::kMap)
        vars.push_back(name);

    return vars;
}
