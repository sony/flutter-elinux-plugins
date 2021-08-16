// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ORIENTATION_MESSAGE_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ORIENTATION_MESSAGE_H_

#include <flutter/binary_messenger.h>
#include <flutter/encodable_value.h>

#include <string>
#include <variant>

#include "types/orientation.h"

class OrientationMessage {
 public:
  OrientationMessage() = default;
  ~OrientationMessage() = default;

  // Prevent copying.
  OrientationMessage(OrientationMessage const&) = default;
  OrientationMessage& operator=(OrientationMessage const&) = default;

  void SetOrientation(DeviceOrientation orientation) {
    orientation_ = orientation;
  }
  DeviceOrientation GetOrientation() const { return orientation_; }

  flutter::EncodableValue ToMap() {
    flutter::EncodableMap map = {
        {flutter::EncodableValue("orientation"),
         flutter::EncodableValue(SerializeDeviceOrientation(orientation_))}};
    return flutter::EncodableValue(map);
  }

  static OrientationMessage FromMap(const flutter::EncodableValue& value) {
    OrientationMessage message;
    if (std::holds_alternative<flutter::EncodableMap>(value)) {
      auto map = std::get<flutter::EncodableMap>(value);

      flutter::EncodableValue& orientation =
          map[flutter::EncodableValue("orientation")];
      if (std::holds_alternative<std::string>(orientation)) {
        message.SetOrientation(
            DeserializeDeviceOrientation(std::get<std::string>(orientation)));
      }
    }
    return message;
  }

 private:
  DeviceOrientation orientation_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_ORIENTATION_MESSAGE_H_
