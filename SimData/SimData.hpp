#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "SimVar.hpp"
#include "pmdg_data.h"

struct PMDGEvent {
    const char* name;
    uint32_t id;
};

struct PMDGEventSet {
    const PMDGEvent* events;
    size_t size;
};

struct PmdgDataConfig {
    const char* name;
    uint32_t dataId;
    uint32_t definitionId;
    uint32_t size;
};

constexpr PmdgDataConfig PMDG_737_CONFIG {
    PMDG_NG3_DATA_NAME,
    PMDG_NG3_DATA_ID,
    PMDG_NG3_DATA_DEFINITION,
    PMDG_NG3_DATA_SIZE
};

constexpr PmdgDataConfig PMDG_777_CONFIG {
    PMDG_777X_DATA_NAME,
    PMDG_777X_DATA_ID,
    PMDG_777X_DATA_DEFINITION,
    PMDG_777X_DATA_SIZE
};

inline const PmdgDataConfig* GetPmdgConfig(PMDGAircraft t) {
    switch (t) {
        case PMDG_737: return &PMDG_737_CONFIG;
        case PMDG_777: return &PMDG_777_CONFIG;
        default: return nullptr;
    }
}

const std::vector<std::string>& GetKnownEvents();
const std::vector<std::string>& GetPmdgEvents(PMDGAircraft planeType = PMDG_737);
uint32_t GetPmdgEventID(const std::string& name, PMDGAircraft planeType = PMDG_737);
const std::vector<std::string>& GetKnownVariables();

// PMDG VARIABLES
enum class PMDGType {
    UnsignedChar,
    Bool,
    Char,
    UnsignedInt,
    Float,
    Int,
    UnsignedShort,
    Short
};

template<typename T>
constexpr PMDGType PMDGTypeMap();

template<> constexpr PMDGType PMDGTypeMap<unsigned char>()  { return PMDGType::UnsignedChar; }
template<> constexpr PMDGType PMDGTypeMap<bool>()           { return PMDGType::Bool; }
template<> constexpr PMDGType PMDGTypeMap<char>()           { return PMDGType::Char; }
template<> constexpr PMDGType PMDGTypeMap<unsigned int>()   { return PMDGType::UnsignedInt; }
template<> constexpr PMDGType PMDGTypeMap<float>()          { return PMDGType::Float; }
template<> constexpr PMDGType PMDGTypeMap<int>()            { return PMDGType::Int; }
template<> constexpr PMDGType PMDGTypeMap<unsigned short>() { return PMDGType::UnsignedShort; }
template<> constexpr PMDGType PMDGTypeMap<short>()          { return PMDGType::Short; }

struct PMDGField {
    std::string name;
    size_t offset;
    PMDGType type;
};

struct PMDGFieldSet {
    const PMDGField* fields;
    size_t size;
};

const std::vector<std::string>& GetPmdgVariables(PMDGAircraft planeType = PMDG_737);
double GetPmdgVarValueAsDouble(const void* dataPtr, const std::string& name, PMDGAircraft planeType = PMDG_737);
