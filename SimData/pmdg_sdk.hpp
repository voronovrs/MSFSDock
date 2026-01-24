#pragma once
#include <cstdint>
#include <vector>

struct PmdgEvent {
    const char* name;
    uint32_t id;
};

const std::vector<PmdgEvent>& GetPmdgEvents();
