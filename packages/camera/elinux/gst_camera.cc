// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gst_camera.h"

#include <iostream>

GstCamera::GstCamera() {
  gst_.pipeline = nullptr;
  gst_.camerabin = nullptr;
  gst_.video_convert = nullptr;
  gst_.video_sink = nullptr;
  gst_.output = nullptr;
  gst_.bus = nullptr;
  gst_.buffer = nullptr;

  if (!CreatePipeline()) {
    std::cerr << "Failed to create a pipeline" << std::endl;
    DestroyPipeline();
    return;
  }

  // Prerolls before getting information from the pipeline.
  Preroll();

  // Sets internal video size and buffier.
  GetVideoSize(width_, height_);
  pixels_.reset(new uint32_t[width_ * height_]);
}

GstCamera::~GstCamera() {
  Stop();
  DestroyPipeline();
}

bool GstCamera::Play() {
  if (gst_element_set_state(gst_.pipeline, GST_STATE_PLAYING) ==
      GST_STATE_CHANGE_FAILURE) {
    std::cerr << "Failed to change the state to PLAYING" << std::endl;
    return false;
  }
  return true;
}

bool GstCamera::Pause() {
  if (gst_element_set_state(gst_.pipeline, GST_STATE_PAUSED) ==
      GST_STATE_CHANGE_FAILURE) {
    std::cerr << "Failed to change the state to PAUSED" << std::endl;
    return false;
  }
  return true;
}

bool GstCamera::Stop() {
  if (gst_element_set_state(gst_.pipeline, GST_STATE_READY) ==
      GST_STATE_CHANGE_FAILURE) {
    std::cerr << "Failed to change the state to READY" << std::endl;
    return false;
  }
  return true;
}

const uint8_t* GstCamera::GetFrameBuffer() {
  std::shared_lock<std::shared_mutex> lock(mutex_buffer_);
  if (!gst_.buffer) {
    return nullptr;
  }

  const uint32_t pixel_bytes = width_ * height_ * 4;
  gst_buffer_extract(gst_.buffer, 0, pixels_.get(), pixel_bytes);
  return reinterpret_cast<const uint8_t*>(pixels_.get());
}

// Creats a camra pipeline using camerabin.
// $ gst-launch-1.0 camerabin viewfinder-sink="videoconvert !
// video/x-raw,format=RGBA ! fakesink"
bool GstCamera::CreatePipeline() {
  gst_.pipeline = gst_pipeline_new("pipeline");
  if (!gst_.pipeline) {
    std::cerr << "Failed to create a pipeline" << std::endl;
    return false;
  }
  gst_.camerabin = gst_element_factory_make("camerabin", "camerabin");
  if (!gst_.camerabin) {
    std::cerr << "Failed to create a source" << std::endl;
    return false;
  }
  gst_.video_convert = gst_element_factory_make("videoconvert", "videoconvert");
  if (!gst_.video_convert) {
    std::cerr << "Failed to create a videoconvert" << std::endl;
    return false;
  }
  gst_.video_sink = gst_element_factory_make("fakesink", "videosink");
  if (!gst_.video_sink) {
    std::cerr << "Failed to create a videosink" << std::endl;
    return false;
  }
  gst_.output = gst_bin_new("output");
  if (!gst_.output) {
    std::cerr << "Failed to create an output" << std::endl;
    return false;
  }
  gst_.bus = gst_pipeline_get_bus(GST_PIPELINE(gst_.pipeline));
  if (!gst_.bus) {
    std::cerr << "Failed to create a bus" << std::endl;
    return false;
  }
  gst_bus_set_sync_handler(gst_.bus, (GstBusSyncHandler)HandleGstMessage, this,
                           NULL);

  // Sets properties to fakesink to get the callback of a decoded frame.
  g_object_set(G_OBJECT(gst_.video_sink), "sync", TRUE, "qos", FALSE, NULL);
  g_object_set(G_OBJECT(gst_.video_sink), "signal-handoffs", TRUE, NULL);
  g_signal_connect(G_OBJECT(gst_.video_sink), "handoff",
                   G_CALLBACK(HandoffHandler), this);
  gst_bin_add_many(GST_BIN(gst_.output), gst_.video_convert, gst_.video_sink,
                   NULL);

  // Adds caps to the converter to convert the color format to RGBA.
  auto* caps = gst_caps_from_string("video/x-raw,format=RGBA");
  auto link_ok =
      gst_element_link_filtered(gst_.video_convert, gst_.video_sink, caps);
  gst_caps_unref(caps);
  if (!link_ok) {
    std::cerr << "Failed to link elements" << std::endl;
    return false;
  }

  auto* sinkpad = gst_element_get_static_pad(gst_.video_convert, "sink");
  auto* ghost_sinkpad = gst_ghost_pad_new("sink", sinkpad);
  gst_pad_set_active(ghost_sinkpad, TRUE);
  gst_element_add_pad(gst_.output, ghost_sinkpad);

  // Sets properties to camerabin.
  g_object_set(gst_.camerabin, "viewfinder-sink", gst_.output, NULL);
  gst_bin_add_many(GST_BIN(gst_.pipeline), gst_.camerabin, NULL);

  return true;
}

void GstCamera::Preroll() {
  if (!gst_.camerabin) {
    return;
  }

  auto result = gst_element_set_state(gst_.pipeline, GST_STATE_PAUSED);
  if (result == GST_STATE_CHANGE_FAILURE) {
    std::cerr << "Failed to change the state to PAUSED" << std::endl;
    return;
  }

  // Waits until the state becomes GST_STATE_PAUSED.
  if (result == GST_STATE_CHANGE_ASYNC) {
    GstState state;
    result =
        gst_element_get_state(gst_.pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
    if (result == GST_STATE_CHANGE_FAILURE) {
      std::cerr << "Failed to get the current state" << std::endl;
    }
  }
}

void GstCamera::DestroyPipeline() {
  if (gst_.video_sink) {
    g_object_set(G_OBJECT(gst_.video_sink), "signal-handoffs", FALSE, NULL);
  }

  if (gst_.pipeline) {
    gst_element_set_state(gst_.pipeline, GST_STATE_NULL);
  }

  if (gst_.buffer) {
    gst_buffer_unref(gst_.buffer);
    gst_.buffer = nullptr;
  }

  if (gst_.bus) {
    gst_object_unref(gst_.bus);
    gst_.bus = nullptr;
  }

  if (gst_.pipeline) {
    gst_object_unref(gst_.pipeline);
    gst_.pipeline = nullptr;
  }

  if (gst_.camerabin) {
    gst_.camerabin = nullptr;
  }

  if (gst_.output) {
    gst_.output = nullptr;
  }

  if (gst_.video_sink) {
    gst_.video_sink = nullptr;
  }

  if (gst_.video_convert) {
    gst_.video_convert = nullptr;
  }
}

void GstCamera::GetVideoSize(int32_t& width, int32_t& height) {
  if (!gst_.pipeline || !gst_.video_sink) {
    std::cerr
        << "Failed to get video size. The pileline hasn't initialized yet.";
    return;
  }

  auto* sink_pad = gst_element_get_static_pad(gst_.video_sink, "sink");
  if (!sink_pad) {
    std::cerr << "Failed to get a pad";
    return;
  }

  auto* caps = gst_pad_get_current_caps(sink_pad);
  auto* structure = gst_caps_get_structure(caps, 0);
  if (!structure) {
    std::cerr << "Failed to get a structure";
    return;
  }

  gst_structure_get_int(structure, "width", &width);
  gst_structure_get_int(structure, "height", &height);
}

// static
void GstCamera::HandoffHandler(GstElement* fakesink, GstBuffer* buf,
                               GstPad* new_pad, gpointer user_data) {
  auto* self = reinterpret_cast<GstCamera*>(user_data);
  auto* caps = gst_pad_get_current_caps(new_pad);
  auto* structure = gst_caps_get_structure(caps, 0);

  int width;
  int height;
  gst_structure_get_int(structure, "width", &width);
  gst_structure_get_int(structure, "height", &height);
  if (width != self->width_ || height != self->height_) {
    self->width_ = width;
    self->height_ = height;
    self->pixels_.reset(new uint32_t[width * height]);
    std::cout << "Pixel buffer size: width = " << width
              << ", height = " << height << std::endl;
  }

  std::lock_guard<std::shared_mutex> lock(self->mutex_buffer_);
  if (self->gst_.buffer) {
    gst_buffer_unref(self->gst_.buffer);
    self->gst_.buffer = nullptr;
  }
  self->gst_.buffer = gst_buffer_ref(buf);
  //self->stream_handler_->OnNotifyFrameDecoded();
}

// static
gboolean GstCamera::HandleGstMessage(GstBus* bus, GstMessage* message,
                                     gpointer user_data) {
  switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_WARNING: {
      gchar* debug;
      GError* error;
      gst_message_parse_warning(message, &error, &debug);
      g_printerr("WARNING from element %s: %s\n", GST_OBJECT_NAME(message->src),
                 error->message);
      g_printerr("Warning details: %s\n", debug);
      g_free(debug);
      g_error_free(error);
      break;
    }
    case GST_MESSAGE_ERROR: {
      gchar* debug;
      GError* error;
      gst_message_parse_error(message, &error, &debug);
      g_printerr("ERROR from element %s: %s\n", GST_OBJECT_NAME(message->src),
                 error->message);
      g_printerr("Error details: %s\n", debug);
      g_free(debug);
      g_error_free(error);
      break;
    }
    default:
      break;
  }
  return TRUE;
}
