// Copyright 2024 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_H_
#define PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_H_

#include <string>

class AudioPlayerStreamHandler {
 public:
  AudioPlayerStreamHandler() = default;
  virtual ~AudioPlayerStreamHandler() = default;

  // Prevent copying.
  AudioPlayerStreamHandler(AudioPlayerStreamHandler const&) = delete;
  AudioPlayerStreamHandler& operator=(AudioPlayerStreamHandler const&) = delete;

  // Notifies the completion of preparation the audio player.
  void OnNotifyPrepared(const std::string &player_id,
                        const bool is_prepared) {
    OnNotifyPreparedInternal(player_id, is_prepared);
  }

  // Notifies the duration of an audio.
  void OnNotifyDuration(const std::string &player_id,
                        const int32_t duration) {
    OnNotifyDurationInternal(player_id, duration);
  }

  // Notifies the completion of seeks an audio.
  void OnNotifySeekCompleted(const std::string &player_id) {
    OnNotifySeekCompletedInternal(player_id);
  }

  // Notifies the completion of playing an audio.
  void OnNotifyPlayCompleted(const std::string &player_id) {
    OnNotifyPlayCompletedInternal(player_id);
  }

  // Notifies the log of the audio player.
  void OnNotifyLog(const std::string &player_id,
                   const std::string &message) {
    OnNotifyLogInternal(player_id, message);
  }

 protected:
  virtual void OnNotifyPreparedInternal(const std::string&, const bool) = 0;
  virtual void OnNotifyDurationInternal(const std::string&, const int32_t) = 0;
  virtual void OnNotifySeekCompletedInternal(const std::string&) = 0;
  virtual void OnNotifyPlayCompletedInternal(const std::string &) = 0;
  virtual void OnNotifyLogInternal(const std::string&, const std::string&) = 0;
};

#endif  // PACKAGES_AUDIOPLAYERS_AUDIOPLAYERS_ELINUX_AUDIO_PLAYER_STREAM_HANDLER_H_
