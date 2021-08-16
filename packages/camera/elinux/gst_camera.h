// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_

#include <gst/gst.h>

#include <memory>
#include <shared_mutex>
#include <string>

#include "camera_stream_handler.h"

class GstCamera {
 public:
  GstCamera(std::unique_ptr<CameraStreamHandler> handler);
  ~GstCamera();

  bool Play();
  bool Pause();
  bool Stop();

  bool SetZoomLevel(float zoom);
  float GetMaxZoomLevel() const { return max_zoom_level_; };
  float GetMinZoomLevel() const { return min_zoom_level_; };

  const uint8_t* GetPreviewFrameBuffer();
  int32_t GetPreviewWidth() const { return width_; };
  int32_t GetPreviewHeight() const { return height_; };

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
  void GetZoomMaxMinSize(float& max, float& min);

  GstCameraElements gst_;
  std::unique_ptr<uint32_t> pixels_;
  int32_t width_ = -1;
  int32_t height_ = -1;
  std::shared_mutex mutex_buffer_;
  std::unique_ptr<CameraStreamHandler> stream_handler_;
  float max_zoom_level_;
  float min_zoom_level_;
  float zoom_level_ = 1.0f;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_
