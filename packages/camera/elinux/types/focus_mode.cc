// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "types/focus_mode.h"

std::string SerializeFocusMode(FocusMode focus_mode) {
  switch (focus_mode) {
    case FocusMode::kLocked:
      return "locked";
    case FocusMode::kAuto:
      return "auto";
    default:
      std::cerr << "Unknown FocusMode value" << std::endl;
      return "auto";
  }
}

FocusMode DeserializeFocusMode(std::string str) {
  if (!str.compare("locked")) {
    return FocusMode::kLocked;
  }
  if (!str.compare("auto")) {
    return FocusMode::kAuto;
  }
  std::cerr << str.c_str() << " is not a valid FocusMode value" << std::endl;
  return FocusMode::kAuto;
}
