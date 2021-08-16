// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_AVAILABLE_CAMERAS_MESSAGE_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_AVAILABLE_CAMERAS_MESSAGE_H_

#include <flutter/binary_messenger.h>
#include <flutter/encodable_value.h>

#include <string>
#include <variant>

class AvailableCamerasMessage {
 public:
  AvailableCamerasMessage() = default;
  ~AvailableCamerasMessage() = default;

  // Prevent copying.
  AvailableCamerasMessage(AvailableCamerasMessage const&) = default;
  AvailableCamerasMessage& operator=(AvailableCamerasMessage const&) = default;

  void SetName(const std::string& name) { name_ = name; }
  std::string GetName() const { return name_; }

  void SetSensorOrientation(const int& sensor_orientation) {
    sensor_orientation_ = sensor_orientation;
  }
  int GetSensorOrientation() const { return sensor_orientation_; }

  void SetLensFacing(const std::string& lens_facing) {
    lens_facing_ = lens_facing;
  }
  std::string GetLensFacing() const { return lens_facing_; }

  flutter::EncodableValue ToMap() {
    flutter::EncodableMap map = {
        {flutter::EncodableValue("name"), flutter::EncodableValue(name_)},
        {flutter::EncodableValue("sensorOrientation"),
         flutter::EncodableValue(sensor_orientation_)},
        {flutter::EncodableValue("lensFacing"),
         flutter::EncodableValue(lens_facing_)}};
    return flutter::EncodableValue(map);
  }

  static AvailableCamerasMessage FromMap(const flutter::EncodableValue& value) {
    AvailableCamerasMessage message;
    if (std::holds_alternative<flutter::EncodableMap>(value)) {
      auto map = std::get<flutter::EncodableMap>(value);

      flutter::EncodableValue& name = map[flutter::EncodableValue("name")];
      if (std::holds_alternative<std::string>(name)) {
        message.SetName(std::get<std::string>(name));
      }

      flutter::EncodableValue& sensor_orientation =
          map[flutter::EncodableValue("sensorOrientation")];
      if (std::holds_alternative<int>(sensor_orientation)) {
        message.SetSensorOrientation(std::get<int>(sensor_orientation));
      }

      flutter::EncodableValue& lens_facing =
          map[flutter::EncodableValue("lensFacing")];
      if (std::holds_alternative<std::string>(lens_facing)) {
        message.SetLensFacing(std::get<std::string>(lens_facing));
      }
    }

    return message;
  }

 private:
  std::string name_;
  int sensor_orientation_;
  std::string lens_facing_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_MESSAGES_AVAILABLE_CAMERAS_MESSAGE_H_
