#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct PmdgEvent {
    const char* name;
    uint32_t id;
};

const std::vector<std::string>& GetKnownEvents();
const std::vector<std::string>& GetPmdgEvents();
uint32_t GetPmdgEventID(const std::string& name);
std::vector<std::string> GetKnownVariables();

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

std::vector<std::string> GetPmdgVariables();
double GetPmdgVarValueAsDouble(const void* dataPtr, const std::string& name);
