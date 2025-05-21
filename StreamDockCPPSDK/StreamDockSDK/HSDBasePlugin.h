//==============================================================================
/**
@file       ESDBasePlugin.h

@brief      Plugin base class

@copyright  (c) 2018, Corsair Memory, Inc.
      This source code is licensed under the MIT-style license found in the
LICENSE file.

**/
//==============================================================================

#pragma once

#include <nlohmann/json.hpp>

class HSDConnectionManager;

class HSDBasePlugin {
 public:
  HSDBasePlugin() {
  }
  virtual ~HSDBasePlugin() {
  }

  void SetConnectionManager(HSDConnectionManager* inConnectionManager) {
    mConnectionManager = inConnectionManager;
  }

  virtual void DidReceiveGlobalSettings(const nlohmann::json& inPayload) {
  }

  virtual void DidReceiveSettings(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void KeyDownForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void KeyUpForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void DialPressForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void DialRotateForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void WillAppearForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void WillDisappearForAction(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

  virtual void DeviceDidConnect(
    const std::string& inDeviceID,
    const nlohmann::json& inDeviceInfo) {
  }

  virtual void DeviceDidDisconnect(const std::string& inDeviceID) {
  }
  
  virtual void SystemDidWakeUp() {
  }

  virtual void SendToPlugin(
    const std::string& inAction,
    const std::string& inContext,
    const nlohmann::json& inPayload,
    const std::string& inDeviceID) {
  }

 protected:
  HSDConnectionManager* mConnectionManager = nullptr;
};
