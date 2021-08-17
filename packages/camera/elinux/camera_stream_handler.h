// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_H_

class CameraStreamHandler {
 public:
  CameraStreamHandler() = default;
  virtual ~CameraStreamHandler() = default;

  // Prevent copying.
  CameraStreamHandler(CameraStreamHandler const&) = delete;
  CameraStreamHandler& operator=(CameraStreamHandler const&) = delete;

  // Notifies the completion of decoding a video frame.
  void OnNotifyFrameDecoded() { OnNotifyFrameDecodedInternal(); }

 protected:
  virtual void OnNotifyFrameDecodedInternal() = 0;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_CAMERA_STREAM_HANDLER_H_
