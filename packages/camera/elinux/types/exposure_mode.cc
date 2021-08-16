// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "types/exposure_mode.h"

std::string SerializeExposureMode(ExposureMode exposure_mode) {
  switch (exposure_mode) {
    case ExposureMode::kLocked:
      return "locked";
    case ExposureMode::kAuto:
      return "auto";
    default:
      return "auto";
      std::cerr << "Unknown ExposureMode value" << std::endl;
  }
}

ExposureMode DeserializeExposureMode(std::string str) {
  if (!str.compare("locked")) {
    return ExposureMode::kLocked;
  }
  if (!str.compare("auto")) {
    return ExposureMode::kAuto;
  }
  std::cerr << str.c_str() << " is not a valid ExposureMode value" << std::endl;
  return ExposureMode::kAuto;
}
