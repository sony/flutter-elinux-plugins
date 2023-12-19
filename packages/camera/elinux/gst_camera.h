// Copyright 2022 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_
#define PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_

#include <gst/gst.h>

#include <functional>
#include <memory>
#include <shared_mutex>
#include <string>

#include "camera_stream_handler.h"

class GstCamera {
 public:
  using OnNotifyCaptured =
      std::function<void(const std::string& captured_file_path)>;

  GstCamera(std::unique_ptr<CameraStreamHandler> handler);
  ~GstCamera();

  static void GstLibraryLoad();
  static void GstLibraryUnload();

  bool Play();
  bool Pause();
  bool Stop();

  void TakePicture(OnNotifyCaptured on_notify_captured);

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
  static GstBusSyncReply HandleGstMessage(GstBus* bus, GstMessage* message,
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
  std::unique_ptr<CameraStreamHandler> stream_handler_ = nullptr;
  float max_zoom_level_;
  float min_zoom_level_;
  float zoom_level_ = 1.0f;
  int captured_count_ = 0;

  OnNotifyCaptured on_notify_captured_ = nullptr;
};

#endif  // PACKAGES_CAMERA_CAMERA_ELINUX_GST_CAMERA_H_
