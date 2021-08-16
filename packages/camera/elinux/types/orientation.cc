// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "types/orientation.h"

std::string SerializeDeviceOrientation(DeviceOrientation orientation) {
  switch (orientation) {
    case DeviceOrientation::kPortraitUp:
      return "portraitUp";
    case DeviceOrientation::kLandscapeLeft:
      return "landscapeLeft";
    case DeviceOrientation::kPortraitDown:
      return "portraitDown";
    case DeviceOrientation::kLandscapeRight:
      return "landscapeRight";
    default:
      std::cerr << "Unknown DeviceOrientation value" << std::endl;
      return "landscapeLeft";
  }
}

DeviceOrientation DeserializeDeviceOrientation(std::string str) {
  if (!str.compare("portraitUp")) {
    return DeviceOrientation::kPortraitUp;
  }
  if (!str.compare("landscapeLeft")) {
    return DeviceOrientation::kLandscapeLeft;
  }
  if (!str.compare("portraitDown")) {
    return DeviceOrientation::kPortraitDown;
  }
  if (!str.compare("landscapeRight")) {
    return DeviceOrientation::kLandscapeRight;
  }
  std::cerr << str.c_str() << " is not a valid DeviceOrientation value"
            << std::endl;
  return DeviceOrientation::kLandscapeLeft;
}
