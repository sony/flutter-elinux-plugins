// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "channels/method_channel_device.h"

#include <flutter/standard_method_codec.h>

namespace {
constexpr char kChannelName[] = "plugins.flutter.io/camera/fromPlatform";
constexpr char kChannelMethodOrientationChanged[] = "orientation_changed";

constexpr char kOrientation[] = "orientation";
};  // namespace

MethodChannelDevice::MethodChannelDevice(flutter::PluginRegistrar* registrar) {
  channel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), kChannelName,
      &flutter::StandardMethodCodec::GetInstance());
}

void MethodChannelDevice::SendDeviceOrientationChangeEvent(
    const DeviceOrientation& orientation) {
  flutter::EncodableMap mp;
  mp[flutter::EncodableValue(kOrientation)] =
      flutter::EncodableValue(SerializeDeviceOrientation(orientation));
  auto value = std::make_unique<flutter::EncodableValue>(mp);

  Send(kChannelMethodOrientationChanged, std::move(value));
}

void MethodChannelDevice::Send(
    const std::string& method,
    std::unique_ptr<flutter::EncodableValue>&& arguments) {
  channel_->InvokeMethod(method, std::move(arguments));
}
