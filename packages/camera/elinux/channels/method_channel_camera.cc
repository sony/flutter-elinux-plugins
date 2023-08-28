// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "channels/method_channel_camera.h"

#include <flutter/standard_method_codec.h>

namespace {
constexpr char kChannelName[] = "plugins.flutter.io/camera/camera";
constexpr char kChannelMethodInitialized[] = "initialized";
};  // namespace

MethodChannelCamera::MethodChannelCamera(flutter::PluginRegistrar* registrar,
                                         int64_t camera_id) {
  std::string channel_name = kChannelName + std::to_string(camera_id);
  channel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), channel_name.c_str(),
      &flutter::StandardMethodCodec::GetInstance());
}

void MethodChannelCamera::SendInitializedEvent(
    CameraInitializedEvent& message) {
  auto value = std::make_unique<flutter::EncodableValue>(message.ToMap());
  Send(kChannelMethodInitialized, std::move(value));
}

void MethodChannelCamera::Send(
    const std::string& method,
    std::unique_ptr<flutter::EncodableValue>&& arguments) {
  channel_->InvokeMethod(method, std::move(arguments));
}
