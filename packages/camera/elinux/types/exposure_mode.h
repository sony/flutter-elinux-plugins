// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_EXPOSURE_MODE_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_EXPOSURE_MODE_H_

#include <iostream>
#include <string>

// See:
// flutter/plugins/packages/camera/camera_platform_interface/lib/src/types/exposure_mode.dart
enum class ExposureMode {
  kAuto,
  kLocked,
};

std::string SerializeExposureMode(ExposureMode exposure_mode);
ExposureMode DeserializeExposureMode(std::string str);

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_EXPOSURE_MODE_H_
