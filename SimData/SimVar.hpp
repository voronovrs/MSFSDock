#pragma once
#include <Windows.h>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include "KnownVariables.hpp"
#include "PMDG_NG3_SDK.h"

enum DEFINITIONS {
    LIVE_VARIABLE = 2,
    FEEDBACK_VARIABLE,
    PMDG_VARIABLE,
    AIR_PATH_REQUEST = 10,
};

enum EVENT_TYPES {
    EVENT_SIM_START = 0,
    EVENT_GENERIC,
    EVENT_PMDG,
};

struct SimVarDefinition {
    std::string name;
    DEFINITIONS group;
    uint16_t used = 0;
    bool registered = false;
    double value = 0.0;

    bool operator==(const SimVarDefinition& other) const {
        return name == other.name;
    }

    bool IsValid() const {
        return !name.empty();
    }

    void IncrementUsage() {
        ++used;
    }

    void DecrementUsage() {
        if (used > 0) {
            --used;
        }
    }

    bool IsInUse() const {
        return used > 0;
    }

    std::string GetUnit() const {
        return KnownVariables::GetUnit(name);
    }

    int GetDecimals() const {
        return KnownVariables::GetDecimals(name);
    }
};

struct SimEventDefinition {
    std::string name;
    std::string uniqueName;
    DWORD id;
    uint16_t used = 0;
    bool registered = false;

    EVENT_TYPES type = EVENT_GENERIC;
    uint32_t pmdgID = 0;
    std::array<uint32_t, 2> pmdgActions;

    bool IsValid() const {
        return !name.empty();
    }

    void IncrementUsage() {
        ++used;
    }

    void DecrementUsage() {
        if (used > 0) {
            --used;
        }
    }

    bool IsInUse() const {
        return used > 0;
    }
};
