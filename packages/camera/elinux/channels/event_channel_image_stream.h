// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_EVENT_CHANNEL_IMAGE_STREAM_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_EVENT_CHANNEL_IMAGE_STREAM_H_

#include <flutter/encodable_value.h>
#include <flutter/event_channel.h>
#include <flutter/plugin_registrar.h>

#include <string>

class EventChannelImageStream {
 public:
  EventChannelImageStream(flutter::PluginRegistrar* registrar);
  ~EventChannelImageStream() = default;

  void Send(const int32_t& width, const int32_t& height, const uint8_t* pixels);

 private:
  std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> channel_;
  std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> event_sink_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_CHANNELS_EVENT_CHANNEL_IMAGE_STREAM_H_
