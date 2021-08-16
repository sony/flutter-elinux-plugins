// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_

#include <functional>

#include "camera_stream_handler.h"

class CameraStreamHandlerImpl : public CameraStreamHandler {
 public:
  using OnNotifyInitialized = std::function<void()>;
  using OnNotifyFrameDecoded = std::function<void()>;
  using OnNotifyCompleted = std::function<void()>;

  CameraStreamHandlerImpl(OnNotifyInitialized on_notify_initialized,
                          OnNotifyFrameDecoded on_notify_frame_decoded,
                          OnNotifyCompleted on_notify_completed)
      : on_notify_initialized_(on_notify_initialized),
        on_notify_frame_decoded_(on_notify_frame_decoded),
        on_notify_completed_(on_notify_completed) {}
  virtual ~CameraStreamHandlerImpl() = default;

  // Prevent copying.
  CameraStreamHandlerImpl(CameraStreamHandlerImpl const&) = delete;
  CameraStreamHandlerImpl& operator=(CameraStreamHandlerImpl const&) = delete;

 protected:
  // |CameraStreamHandler|
  void OnNotifyInitializedInternal() {
    if (on_notify_initialized_) {
      on_notify_initialized_();
    }
  }

  // |CameraStreamHandler|
  void OnNotifyFrameDecodedInternal() {
    if (on_notify_frame_decoded_) {
      on_notify_frame_decoded_();
    }
  }

  // |CameraStreamHandler|
  void OnNotifyCompletedInternal() {
    if (on_notify_completed_) {
      on_notify_completed_();
    }
  }

  OnNotifyInitialized on_notify_initialized_;
  OnNotifyFrameDecoded on_notify_frame_decoded_;
  OnNotifyCompleted on_notify_completed_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_IMPL_H_
