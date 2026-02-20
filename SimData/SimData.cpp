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

// COMMON VARIABLE NAMES FOR UI
std::vector<std::string> GetKnownVariables() {
    std::vector<std::string> vars;
    vars.reserve(KnownVariables::kMap.size());

    for (const auto& [name, _] : KnownVariables::kMap)
        vars.push_back(name);

    return vars;
}

// PMDG VARIABLES
static const PMDGField PMDG_FIELDS[] = {
#define X(type, name, offset) { #name, offset, PMDGTypeMap<type>() },
#include "pmdg_variables.inc"
#undef X
};

// PMDG VARIABLE MAP LOOKUP
static std::unordered_map<std::string, const PMDGField*> BuildFieldMap() {
    std::unordered_map<std::string, const PMDGField*> map;
    map.reserve(sizeof(PMDG_FIELDS) / sizeof(PMDG_FIELDS[0]));

    for (const auto& f : PMDG_FIELDS)
        map.emplace(f.name, &f);

    return map;
}

static const auto PMDG_FIELD_MAP = BuildFieldMap();

// PMDG VARIABLES NAMES FOR UI
std::vector<std::string> GetPmdgVariables() {
    std::vector<std::string> names;
    names.reserve(sizeof(PMDG_FIELDS) / sizeof(PMDG_FIELDS[0]));

    for (const auto& f : PMDG_FIELDS)
        names.emplace_back(f.name);

    return names;
}

// GET PMDG VARIABLE VALUE BY NAME
double GetPmdgVarValueAsDouble(const void* dataPtr, const std::string& name) {
    auto it = PMDG_FIELD_MAP.find(name);
    if (it == PMDG_FIELD_MAP.end()) {
        LogError("Unknown PMDG variable: " + name);
        return 0.0;
    }

    const PMDGField* f = it->second;
    const char* base = reinterpret_cast<const char*>(dataPtr);
    const void* ptr = base + f->offset;

    switch (f->type) {
        case PMDGType::Bool:          return static_cast<double>(*reinterpret_cast<const bool*>(ptr));
        case PMDGType::Char:          return static_cast<double>(*reinterpret_cast<const char*>(ptr));
        case PMDGType::UnsignedChar:  return static_cast<double>(*reinterpret_cast<const unsigned char*>(ptr));
        case PMDGType::Short:         return static_cast<double>(*reinterpret_cast<const short*>(ptr));
        case PMDGType::UnsignedShort: return static_cast<double>(*reinterpret_cast<const unsigned short*>(ptr));
        case PMDGType::Int:           return static_cast<double>(*reinterpret_cast<const int*>(ptr));
        case PMDGType::UnsignedInt:   return static_cast<double>(*reinterpret_cast<const unsigned int*>(ptr));
        case PMDGType::Float:         return static_cast<double>(*reinterpret_cast<const float*>(ptr));
    }

    return 0.0;
}
