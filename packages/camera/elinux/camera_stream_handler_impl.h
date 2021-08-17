// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_

#include <functional>

#include "camera_stream_handler.h"

class CameraStreamHandlerImpl : public CameraStreamHandler {
 public:
  using OnNotifyFrameDecoded = std::function<void()>;

  CameraStreamHandlerImpl(OnNotifyFrameDecoded on_notify_frame_decoded)
      : on_notify_frame_decoded_(on_notify_frame_decoded) {}
  virtual ~CameraStreamHandlerImpl() = default;

  // Prevent copying.
  CameraStreamHandlerImpl(CameraStreamHandlerImpl const&) = delete;
  CameraStreamHandlerImpl& operator=(CameraStreamHandlerImpl const&) = delete;

 protected:
  // |CameraStreamHandler|
  void OnNotifyFrameDecodedInternal() {
    if (on_notify_frame_decoded_) {
      on_notify_frame_decoded_();
    }
  }

  OnNotifyFrameDecoded on_notify_frame_decoded_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_
