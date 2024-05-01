// Copyright 2024 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_GST_AUDIO_PLAYER_H_
#define PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_GST_AUDIO_PLAYER_H_

#include <gst/gst.h>

#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

#include "audio_player_stream_handler.h"

class GstAudioPlayer {
 public:
  GstAudioPlayer(const std::string &player_id,
                 std::unique_ptr<AudioPlayerStreamHandler> handler);
  ~GstAudioPlayer();

  static void GstLibraryLoad();
  static void GstLibraryUnload();

  void Resume();
  void Play();
  void Pause();
  void Stop();
  void Seek(int64_t position); 
  void SetSourceUrl(std::string url);
  void SetVolume(double volume);
  void SetBalance(double balance);
  void SetPlaybackRate(double playback_rate);
  void SetLooping(bool is_looping);
  int64_t GetDuration();
  int64_t GetCurrentPosition();
  void Release();
  void Dispose();

 private:
  struct GstAudioElements {
    GstElement* playbin;
    GstBus* bus;
    GstElement* source;
    GstElement* panorama;
    GstElement* audiobin;
    GstElement* audiosink;
    GstPad* panoramasinkpad;
  };

  static GstBusSyncReply HandleGstMessage(GstBus* bus, GstMessage* message,
                                          gpointer user_data);
  static void SourceSetup(GstElement* playbin,
                          GstElement* source,
                          GstElement** p_src);
  bool CreatePipeline();
  std::string ParseUri(const std::string& uri);

  GstAudioElements gst_;
  const std::string player_id_;
  std::string url_;
  bool is_initialized_ = false;
  bool is_playing_ = false;
  bool is_looping_ = false;
  double volume_ = 1.0;
  double playback_rate_ = 1.0;
  bool is_completed_ = false;
  std::unique_ptr<AudioPlayerStreamHandler> stream_handler_;
};

#endif  // PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_GST_AUDIO_PLAYER_H_
