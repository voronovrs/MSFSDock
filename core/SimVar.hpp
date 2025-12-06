#pragma once
#include <Windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include "KnownVariables.hpp"

enum DEFINITIONS {
    LIVE_VARIABLE = 2,
    FEEDBACK_VARIABLE,
    EVENT_GROUP,
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
    DWORD id;
    uint16_t used = 0;
    bool registered = false;

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
