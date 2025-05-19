#include <Windows.h>
#include <SimConnect\SimConnect.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "Logger.h"
#include "Sim.h"

HANDLE hSimConnect = nullptr;

enum DATA_DEFINE_ID {
    DEFINITION_1,
    DEFINITION_2
    };
enum DATA_REQUEST_ID {
    REQUEST_1,
    REQUEST_2,
    };
struct Struct1
    {
    double  kohlsmann;
    double  altitude;
    double  latitude;
    double  longitude;
    };

void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
    // Handle received SimConnect messages
    LogInfo("Recieved dwID " + std::to_string(pData->dwID));
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_QUIT:
            LogWarn("MSFS has closed or disconnected.");
            // Signal your thread to exit or try reconnect
            hSimConnect = nullptr;
            break;
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
            {
            SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*) pData;
            switch(pObjData->dwRequestID)
                {
                case REQUEST_1:
                    Struct1 *pS = (Struct1*)&pObjData->dwData;
                    LogInfo("Recieved data " + std::to_string(pS->kohlsmann));
                    break;
                }
            }
            break;

        // Handle other message types here

        default:
            break;
    }
}


void RunSimConnectLoop() {
    bool added = false;
    while (true) {
        if (hSimConnect == nullptr) {
            HRESULT hr = SimConnect_Open(&hSimConnect, "MSFSDock Plugin", nullptr, 0, 0, 0);
            if (hr != S_OK) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                LogInfo("Waiting for SimConnect to run");
                continue; // Retry until connected
            }
            LogInfo("SimConnect connected");
            // Setup subscriptions, data requests, etc.
        }

        // Dispatch messages
        if (hSimConnect) {
            HRESULT hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProc, nullptr);
            if (FAILED(hr)) {
                LogError("SimConnect_CallDispatch failed, assumed disconnected");
                hSimConnect = nullptr;
            }
        }

        if (hSimConnect) {
            if (!added) {
                HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR:3", "feet");
                if (FAILED(hr)) {
                    LogError("SimConnect_AddToDataDefinition failed");
                } else {
                    added = true;
                }
            }
        }

        if (hSimConnect && added) {
            HRESULT hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
