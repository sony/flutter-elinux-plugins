// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_ORIENTATION_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_ORIENTATION_H_

#include <iostream>
#include <string>

// See: [DeviceOrientation] in
// flutter/packages/flutter/lib/src/services/system_chrome.dart
enum class DeviceOrientation {
  kPortraitUp,
  kLandscapeLeft,
  kPortraitDown,
  kLandscapeRight,
};

std::string SerializeDeviceOrientation(DeviceOrientation orientation);
DeviceOrientation DeserializeDeviceOrientation(std::string str);

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_TYPES_ORIENTATION_H_
