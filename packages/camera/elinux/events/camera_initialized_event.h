// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_EVENT_CAMERA_INITIALIZED_EVENT_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_EVENT_CAMERA_INITIALIZED_EVENT_H_

#include <flutter/binary_messenger.h>
#include <flutter/encodable_value.h>

#include <string>
#include <variant>

#include "types/exposure_mode.h"
#include "types/focus_mode.h"

// See:
// flutter/plugins/packages/camera/camera_platform_interface/lib/src/events/camera_event.dart
class CameraInitializedEvent {
 public:
  CameraInitializedEvent() = default;
  ~CameraInitializedEvent() = default;

  // Prevent copying.
  CameraInitializedEvent(CameraInitializedEvent const&) = default;
  CameraInitializedEvent& operator=(CameraInitializedEvent const&) = default;

  void SetPreviewWidth(const double& width) { preview_width_ = width; }
  double GetPreviewWidth() const { return preview_width_; }

  void SetPreviewHeight(const double& height) { preview_height_ = height; }
  double GetPreviewHeight() const { return preview_height_; }

  void SetFocusMode(const FocusMode& focus_mode) { focus_mode_ = focus_mode; }
  FocusMode GetFocusMode() const { return focus_mode_; }

  void SetExposureMode(const ExposureMode& exposure_mode) {
    exposure_mode_ = exposure_mode;
  }
  ExposureMode GetExposureMode() const { return exposure_mode_; }

  void SetFocusPointSupported(const bool& supported) {
    focus_point_supported_ = supported;
  }
  bool GetFocusPointSupported() const { return focus_point_supported_; }

  void SetExposurePointSupported(const bool& supported) {
    exposure_point_supported_ = supported;
  }
  bool GetExposurePointSupported() const { return exposure_point_supported_; }

  flutter::EncodableValue ToMap() {
    flutter::EncodableMap map = {
        {flutter::EncodableValue("previewWidth"),
         flutter::EncodableValue(preview_width_)},
        {flutter::EncodableValue("previewHeight"),
         flutter::EncodableValue(preview_height_)},
        {flutter::EncodableValue("focusMode"),
         flutter::EncodableValue(SerializeFocusMode(focus_mode_))},
        {flutter::EncodableValue("exposureMode"),
         flutter::EncodableValue(SerializeExposureMode(exposure_mode_))},
        {flutter::EncodableValue("focusPointSupported"),
         flutter::EncodableValue(focus_point_supported_)},
        {flutter::EncodableValue("exposurePointSupported"),
         flutter::EncodableValue(exposure_point_supported_)},
    };
    return flutter::EncodableValue(map);
  }

  static CameraInitializedEvent FromMap(const flutter::EncodableValue& value) {
    CameraInitializedEvent message;
    if (std::holds_alternative<flutter::EncodableMap>(value)) {
      auto map = std::get<flutter::EncodableMap>(value);

      flutter::EncodableValue& preview_width =
          map[flutter::EncodableValue("previewWidth")];
      if (std::holds_alternative<double>(preview_width)) {
        message.SetPreviewWidth(std::get<double>(preview_width));
      }

      flutter::EncodableValue& preview_height =
          map[flutter::EncodableValue("previewHeight")];
      if (std::holds_alternative<double>(preview_height)) {
        message.SetPreviewHeight(std::get<double>(preview_height));
      }

      flutter::EncodableValue& focus_mode =
          map[flutter::EncodableValue("focusMode")];
      if (std::holds_alternative<std::string>(focus_mode)) {
        message.SetFocusMode(
            DeserializeFocusMode(std::get<std::string>(focus_mode)));
      }

      flutter::EncodableValue& exposure_mode =
          map[flutter::EncodableValue("exposureMode")];
      if (std::holds_alternative<std::string>(exposure_mode)) {
        message.SetExposureMode(
            DeserializeExposureMode(std::get<std::string>(exposure_mode)));
      }

      flutter::EncodableValue& focus_point_supported =
          map[flutter::EncodableValue("focusPointSupported")];
      if (std::holds_alternative<bool>(focus_point_supported)) {
        message.SetFocusPointSupported(std::get<bool>(focus_point_supported));
      }

      flutter::EncodableValue& exposure_point_supported =
          map[flutter::EncodableValue("exposurePointSupported")];
      if (std::holds_alternative<bool>(exposure_point_supported)) {
        message.SetExposurePointSupported(
            std::get<bool>(exposure_point_supported));
      }
    }

    return message;
  }

 private:
  double preview_width_;
  double preview_height_;
  FocusMode focus_mode_;
  ExposureMode exposure_mode_;
  bool focus_point_supported_;
  bool exposure_point_supported_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_EVENT_CAMERA_INITIALIZED_EVENT_H_
