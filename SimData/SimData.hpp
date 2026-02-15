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
