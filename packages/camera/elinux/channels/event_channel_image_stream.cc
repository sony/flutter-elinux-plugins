// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "channels/event_channel_image_stream.h"

#include <flutter/event_stream_handler_functions.h>
#include <flutter/standard_method_codec.h>

#include <vector>

namespace {
constexpr char kChannelName[] = "plugins.flutter.io/camera/imageStream";

// See: [getFormat()] in
// https://developer.android.com/reference/android/media/Image
constexpr int32_t kImageFormatRGBA8888 = 4;
};  // namespace

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

// See: [setImageStreamImageAvailableListener] in
// flutter/plugins/packages/camera/camera/android/src/main/java/io/flutter/plugins/camera/Camera.java
void EventChannelImageStream::Send(const int32_t& width, const int32_t& height,
                                   const uint8_t* pixels) {
  const uint32_t len = width * 4 * height;
  std::vector<uint8_t> bytes(pixels, pixels + len);

  flutter::EncodableList planes;
  flutter::EncodableMap plane = {
      {flutter::EncodableValue("bytesPerRow"), flutter::EncodableValue(width)},
      {flutter::EncodableValue("bytesPerPixel"), flutter::EncodableValue(4)},
      {flutter::EncodableValue("bytes"), flutter::EncodableValue(bytes)},
  };
  flutter::EncodableValue plane_value(plane);
  planes.push_back(plane_value);

  flutter::EncodableMap encodables = {
      {flutter::EncodableValue("width"), flutter::EncodableValue(width)},
      {flutter::EncodableValue("height"), flutter::EncodableValue(height)},
      {flutter::EncodableValue("format"),
       flutter::EncodableValue(kImageFormatRGBA8888)},
      {flutter::EncodableValue("planes"), flutter::EncodableValue(planes)}};
  flutter::EncodableValue event(encodables);

  event_sink_->Success(event);
}
