// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/joystick/joystick_plugin.h"

#include <flutter/plugin_registrar.h>

#include <memory>
#include <sstream>

namespace {

class JoystickPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

  JoystickPlugin();

  virtual ~JoystickPlugin();
};

// static
void JoystickPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  auto plugin = std::make_unique<JoystickPlugin>();
  registrar->AddPlugin(std::move(plugin));
}

JoystickPlugin::JoystickPlugin() {}

JoystickPlugin::~JoystickPlugin() {}

}  // namespace

void JoystickPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  JoystickPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrar>(registrar));
}
