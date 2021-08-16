// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ZOOM_LEVEL_MESSAGE_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ZOOM_LEVEL_MESSAGE_H_

#include <flutter/binary_messenger.h>
#include <flutter/encodable_value.h>

#include <variant>

class ZoomLevelMessage {
 public:
  ZoomLevelMessage() = default;
  ~ZoomLevelMessage() = default;

  // Prevent copying.
  ZoomLevelMessage(ZoomLevelMessage const&) = default;
  ZoomLevelMessage& operator=(ZoomLevelMessage const&) = default;

  void SetZoom(double zoom) { zoom_ = zoom; }
  double GetZoom() const { return zoom_; }

  flutter::EncodableValue ToMap() {
    flutter::EncodableMap map = {
        {flutter::EncodableValue("zoom"), flutter::EncodableValue(zoom_)}};
    return flutter::EncodableValue(map);
  }

  static ZoomLevelMessage FromMap(const flutter::EncodableValue& value) {
    ZoomLevelMessage message;
    if (std::holds_alternative<flutter::EncodableMap>(value)) {
      auto map = std::get<flutter::EncodableMap>(value);

      flutter::EncodableValue& zoom = map[flutter::EncodableValue("zoom")];
      if (std::holds_alternative<double>(zoom)) {
        message.SetZoom(std::get<double>(zoom));
      }
    }
    return message;
  }

 private:
  double zoom_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ZOOM_LEVEL_MESSAGE_H_
