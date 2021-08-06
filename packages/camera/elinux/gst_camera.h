// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_GST_VIDEO_PLAYER_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_GST_VIDEO_PLAYER_H_

#include <gst/gst.h>

#include <memory>
#include <shared_mutex>
#include <string>

class GstCamera {
 public:
  GstCamera();
  ~GstCamera();

  bool Play();
  bool Pause();
  bool Stop();

  const uint8_t* GetFrameBuffer();
  int32_t GetWidth() const { return width_; };
  int32_t GetHeight() const { return height_; };

 private:
  struct GstCameraElements {
    GstElement* pipeline;
    GstElement* camerabin;
    GstElement* video_convert;
    GstElement* video_sink;
    GstElement* output;
    GstBus* bus;
    GstBuffer* buffer;
  };

  static void HandoffHandler(GstElement* fakesink, GstBuffer* buf,
                             GstPad* new_pad, gpointer user_data);
  static gboolean HandleGstMessage(GstBus* bus, GstMessage* message,
                                   gpointer user_data);

  bool CreatePipeline();
  void DestroyPipeline();
  void Preroll();
  void GetVideoSize(int32_t& width, int32_t& height);

  GstCameraElements gst_;
  std::unique_ptr<uint32_t> pixels_;
  int32_t width_;
  int32_t height_;
  std::shared_mutex mutex_buffer_;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_GST_VIDEO_PLAYER_H_
