#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"

inline int getIntFromJson(const nlohmann::json& settings, const std::string& key, int defaultValue) {
    try {
        if (settings.contains(key)) {
            if (settings[key].is_number_integer()) return settings[key];
            if (settings[key].is_string()) return std::stoi(settings[key].get<std::string>());
        }
    } catch (...) {
        return defaultValue;
    }
    return defaultValue;
}

inline float getFloatFromJson(const nlohmann::json& settings, const std::string& key, float defaultValue) {
    try {
        if (settings.contains(key)) {
            if (settings[key].is_number()) return settings[key].get<float>();
            if (settings[key].is_string()) return std::stof(settings[key].get<std::string>());
        }
    } catch (...) {
        return defaultValue;
    }
    return defaultValue;
}

inline std::string doubleToStr(double v, bool integer) {
    if (integer)
        return std::to_string(static_cast<int>(v));

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << v;
    return oss.str();
}
