// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_METHOD_CHANNEL_DEVICE_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_METHOD_CHANNEL_DEVICE_H_

#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>

#include <string>

#include "types/orientation.h"

class MethodChannelDevice {
 public:
  MethodChannelDevice(flutter::PluginRegistrar* registrar);
  ~MethodChannelDevice() = default;

  void SendDeviceOrientationChangeEvent(const DeviceOrientation& orientation);

 private:
  void Send(const std::string& method,
            std::unique_ptr<flutter::EncodableValue>&& arguments);

  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_METHOD_CHANNEL_DEVICE_H_
