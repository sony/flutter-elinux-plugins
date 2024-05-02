// Copyright 2024 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_IMPL_H_
#define PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_IMPL_H_

#include <functional>

#include "audio_player_stream_handler.h"

class AudioPlayerStreamHandlerImpl : public AudioPlayerStreamHandler {
 public:
  using OnNotifyPrepared = std::function<void(const std::string&, const bool)>;
  using OnNotifyDuration =
      std::function<void(const std::string&, const int32_t)>;
  using OnNotifySeekCompleted = std::function<void(const std::string&)>;
  using OnNotifyPlayCompleted = std::function<void(const std::string&)>;
  using OnNotifyLog =
        std::function<void(const std::string&, const std::string&)>;

  AudioPlayerStreamHandlerImpl(OnNotifyPrepared on_notify_prepared,
                               OnNotifyDuration on_notify_duration,
                               OnNotifySeekCompleted on_notify_seek_completed,
                               OnNotifyPlayCompleted on_notify_play_completed,
                               OnNotifyLog on_notify_log)
      : on_notify_prepared_(on_notify_prepared),
        on_notify_duration_(on_notify_duration),
        on_notify_seek_completed_(on_notify_seek_completed),
        on_notify_play_completed_(on_notify_play_completed),
        on_notify_log_(on_notify_log) {}
  virtual ~AudioPlayerStreamHandlerImpl() = default;

  // Prevent copying.
  AudioPlayerStreamHandlerImpl(AudioPlayerStreamHandlerImpl const&) = delete;
  AudioPlayerStreamHandlerImpl& operator=(AudioPlayerStreamHandlerImpl const&) =
      delete;

 protected:
  // |AudioPlayerStreamHandler|
  void OnNotifyPreparedInternal(const std::string &player_id,
                                const bool is_prepared) {
    if (on_notify_prepared_) {
      on_notify_prepared_(player_id, is_prepared);
    }
  }

  // |AudioPlayerStreamHandler|
  void OnNotifyDurationInternal(const std::string &player_id,
                                const int32_t duration) {
    if (on_notify_duration_) {
      on_notify_duration_(player_id, duration);
    }
  }

  // |AudioPlayerStreamHandler|
  void OnNotifySeekCompletedInternal(const std::string &player_id) {
    if (on_notify_seek_completed_) {
      on_notify_seek_completed_(player_id);
    }
  }

  // |AudioPlayerStreamHandler|
  void OnNotifyPlayCompletedInternal(const std::string &player_id) {
    if (on_notify_play_completed_) {
      on_notify_play_completed_(player_id);
    }
  }

  // |AudioPlayerStreamHandler|
  void OnNotifyLogInternal(const std::string &player_id,
                           const std::string &message) {
    if (on_notify_log_) {
      on_notify_log_(player_id, message);
    }
  }

  OnNotifyPrepared on_notify_prepared_;
  OnNotifyDuration on_notify_duration_;
  OnNotifySeekCompleted on_notify_seek_completed_;
  OnNotifyPlayCompleted on_notify_play_completed_;
  OnNotifyLog on_notify_log_;
};

#endif  // PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_IMPL_H_
