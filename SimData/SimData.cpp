#include "plugin/Logger.hpp"
#include "SimData.hpp"
#include "KnownVariables.hpp"

// ------------- EVENTS --------------

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
#define X(name, id) { #name, id },

static const PMDGEvent PMDG_737_EVENTS[] = {
#include "pmdg_events_737.inc"
};

static const PMDGEvent PMDG_777_EVENTS[] = {
#include "pmdg_events_777.inc"
};

#undef X

constexpr size_t PMDG_737_EVENT_COUNT = sizeof(PMDG_737_EVENTS) / sizeof(PMDG_737_EVENTS[0]);
constexpr size_t PMDG_777_EVENT_COUNT = sizeof(PMDG_777_EVENTS) / sizeof(PMDG_777_EVENTS[0]);

inline PMDGEventSet GetEventSet(PMDGAircraft t) {
    return (t == PMDG_737)
        ? PMDGEventSet{ PMDG_737_EVENTS, PMDG_737_EVENT_COUNT }
        : PMDGEventSet{ PMDG_777_EVENTS, PMDG_777_EVENT_COUNT };
}

const std::vector<std::string>& GetPmdgEvents(PMDGAircraft planeType) {
    static const std::vector<std::string> PMDG_737_NAMES = [] {
        std::vector<std::string> v;
        v.reserve(PMDG_737_EVENT_COUNT);
        for (size_t i = 0; i < PMDG_737_EVENT_COUNT; ++i)
            v.emplace_back(PMDG_737_EVENTS[i].name);
        return v;
    }();

    static const std::vector<std::string> PMDG_777_NAMES = [] {
        std::vector<std::string> v;
        v.reserve(PMDG_777_EVENT_COUNT);
        for (size_t i = 0; i < PMDG_777_EVENT_COUNT; ++i)
            v.emplace_back(PMDG_777_EVENTS[i].name);
        return v;
    }();

    return (planeType == PMDG_737) ? PMDG_737_NAMES : PMDG_777_NAMES;
}

uint32_t GetPmdgEventID(const std::string& name, PMDGAircraft planeType) {
    auto set = GetEventSet(planeType);

    for (size_t i = 0; i < set.size; ++i) {
        if (name == set.events[i].name)
            return set.events[i].id;
    }

    LogError("Can't find ID for PMDG event " + name);
    return THIRD_PARTY_EVENT_ID_MIN;
}

// ------------- VARIABLES --------------
// COMMON VARIABLE NAMES FOR UI
const std::vector<std::string>& GetKnownVariables() {
    static const std::vector<std::string> KNOWN_VARIABLE_NAMES = [] {
        std::vector<std::string> v;
        v.reserve(KnownVariables::kMap.size());
        for (const auto& [name, _] : KnownVariables::kMap)
            v.push_back(name);
        return v;
    }();

    return KNOWN_VARIABLE_NAMES;
}

// PMDG VARIABLES
#define X(type, name, offset) { #name, offset, PMDGTypeMap<type>() },

static const PMDGField PMDG_FIELDS_737[] = {
#include "pmdg_variables_737.inc"
};

static const PMDGField PMDG_FIELDS_777[] = {
#include "pmdg_variables_777.inc"
};

#undef X

constexpr PMDGFieldSet PMDG_737_FIELD_SET {
    PMDG_FIELDS_737,
    sizeof(PMDG_FIELDS_737) / sizeof(PMDG_FIELDS_737[0])
};

constexpr PMDGFieldSet PMDG_777_FIELD_SET {
    PMDG_FIELDS_777,
    sizeof(PMDG_FIELDS_777) / sizeof(PMDG_FIELDS_777[0])
};

// PMDG VARIABLE MAP LOOKUP
static std::unordered_map<std::string, const PMDGField*> BuildFieldMap(PMDGAircraft planeType) {
    std::unordered_map<std::string, const PMDGField*> map;

    auto set = (planeType == PMDG_737) ? PMDG_737_FIELD_SET : PMDG_777_FIELD_SET;
    map.reserve(set.size);

    for (size_t i = 0; i < set.size; ++i) {
        const auto& f = set.fields[i];
        map.emplace(f.name, &f);
    }

    return map;
}

static const auto PMDG_FIELD_737_MAP = BuildFieldMap(PMDG_737);
static const auto PMDG_FIELD_777_MAP = BuildFieldMap(PMDG_777);

// PMDG VARIABLES NAMES FOR UI
const std::vector<std::string>& GetPmdgVariables(PMDGAircraft planeType) {
    static const std::vector<std::string> PMDG_737_VAR_NAMES = [] {
        std::vector<std::string> v;
        v.reserve(PMDG_737_FIELD_SET.size);
        for (size_t i = 0; i < PMDG_737_FIELD_SET.size; ++i)
            v.emplace_back(PMDG_737_FIELD_SET.fields[i].name);
        return v;
    }();

    static const std::vector<std::string> PMDG_777_VAR_NAMES = [] {
        std::vector<std::string> v;
        v.reserve(PMDG_777_FIELD_SET.size);
        for (size_t i = 0; i < PMDG_777_FIELD_SET.size; ++i)
            v.emplace_back(PMDG_777_FIELD_SET.fields[i].name);
        return v;
    }();

    return (planeType == PMDG_737) ? PMDG_737_VAR_NAMES : PMDG_777_VAR_NAMES;
}

// GET PMDG VARIABLE VALUE BY NAME
double GetPmdgVarValueAsDouble(const void* dataPtr, const std::string& name, PMDGAircraft planeType) {
    if (planeType == PMDG_NONE) {
        LogWarn("Requesting data " + name + " for unregistered PMDG plane");
        return 0.0;
    }
    const auto& field_map = (planeType == PMDG_737) ? PMDG_FIELD_737_MAP : PMDG_FIELD_777_MAP;

    auto it = field_map.find(name);
    if (it == field_map.end()) {
        LogError("Unknown PMDG variable: " + name + " plane_type: " + ((planeType == PMDG_737) ? "737" : "777"));
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
