#pragma once
#include <string>
#include <vector>
#include <SimConnect.h>
#include "Logger.h"

struct SimVarDefinition {
    std::string name;
    std::string unit;
};

struct SimGroup {
    uint32_t definitionId;
    uint32_t requestId;
    std::vector<SimVarDefinition> variables;

    void Register(HANDLE hSimConnect) const {
        for (const auto& var : variables) {
            HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, definitionId, var.name.c_str(), var.unit.c_str(), SIMCONNECT_DATATYPE_FLOAT64);
            if (FAILED(hr)) {
                LogError("Failed to register sim var: " + var.name);
            } else {
                LogInfo("Registered sim var: " + var.name);
            }
        }
    }

    void DeRegister(HANDLE hSimConnect) const {
        LogInfo("Try to deregister sim vars with ID " + std::to_string(definitionId));
        HRESULT hr = SimConnect_ClearDataDefinition(hSimConnect, definitionId);
        if (FAILED(hr)) {
            LogError("Failed to deregister sim vars.");
        } else {
            LogInfo("Sim vars deregistered.");
        }
    }

    void ParseValues(const SIMCONNECT_RECV_SIMOBJECT_DATA* data) const {
        const BYTE* raw = reinterpret_cast<const BYTE*>(&data->dwData);
        for (const auto& var : variables) {
            double val = *reinterpret_cast<const double*>(raw);
            LogInfo(var.name + " = " + std::to_string(val));
            raw += sizeof(double);
        }
    }
};
