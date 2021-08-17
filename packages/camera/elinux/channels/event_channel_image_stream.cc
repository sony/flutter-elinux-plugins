// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "channels/event_channel_image_stream.h"

#include <flutter/event_stream_handler_functions.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <iostream>
#include <cstdint>
#include <cstring>

namespace {
constexpr char kChannelName[] = "plugins.flutter.io/camera/imageStream";
};  // namespace

// See:
// flutter/plugins/packages/camera/camera/android/src/main/java/io/flutter/plugins/camera/Camera.java
EventChannelImageStream::EventChannelImageStream(
    flutter::PluginRegistrar* registrar) {
  auto event_channel =
      std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
          registrar->messenger(), kChannelName,
          &flutter::StandardMethodCodec::GetInstance());

  auto event_channel_handler = std::make_unique<
      flutter::StreamHandlerFunctions<flutter::EncodableValue>>(
      [this](
          const flutter::EncodableValue* arguments,
          std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events)
          -> std::unique_ptr<
              flutter::StreamHandlerError<flutter::EncodableValue>> {
        event_sink_ = std::move(events);
        return nullptr;
      },
      [this](const flutter::EncodableValue* arguments)
          -> std::unique_ptr<
              flutter::StreamHandlerError<flutter::EncodableValue>> {
        event_sink_ = nullptr;
        return nullptr;
      });
  event_channel->SetStreamHandler(std::move(event_channel_handler));
}

void EventChannelImageStream::Send(const int32_t& width, const int32_t& height,
                                   const uint8_t* pixels) {
  const uint32_t len = width * 4 * height;
  std::unique_ptr<uint8_t> bytes;
  bytes.reset(new uint8_t[len]);
  std::memcpy(&bytes, pixels, len);

  flutter::EncodableMap planes = {
      {flutter::EncodableValue("bytesPerRow"), flutter::EncodableValue(width)},
      {flutter::EncodableValue("bytesPerPixel"), flutter::EncodableValue(4)},
      {flutter::EncodableValue("bytes"), flutter::EncodableValue(bytes.get())},
  };

  // See:
  // https://developer.android.com/reference/android/media/Image#getFormat()
  constexpr int32_t format = 4; /*FLEX_RGBA_8888*/
  flutter::EncodableMap encodables = {
      {flutter::EncodableValue("width"), flutter::EncodableValue(width)},
      {flutter::EncodableValue("height"), flutter::EncodableValue(height)},
      {flutter::EncodableValue("format"), flutter::EncodableValue(format)},
      {flutter::EncodableValue("planes"), flutter::EncodableValue(planes)}};
  flutter::EncodableValue event(encodables);

  event_sink_->Success(event);
}
