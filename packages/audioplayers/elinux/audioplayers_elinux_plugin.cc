// Copyright 2024 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/audioplayers_elinux/audioplayers_elinux_plugin.h"

#include <flutter/basic_message_channel.h>
#include <flutter/encodable_value.h>
#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_message_codec.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <string>
#include <variant>

#include "gst_audio_player.h"
#include "audio_player_stream_handler_impl.h"

namespace {
constexpr char kInvalidArgument[] = "Invalid argument";
constexpr char kAudioDurationEvent[] = "audio.onDuration";
constexpr char kAudioPreparedEvent[] = "audio.onPrepared";
constexpr char kAudioSeekCompleteEvent[] = "audio.onSeekComplete";
constexpr char kAudioCompleteEvent[] = "audio.onComplete";
constexpr char kAudioLogEvent[] = "audio.onLog";

template <typename T>
bool GetValueFromEncodableMap(const flutter::EncodableMap* map, const char* key,
                              T &out) {
  auto iter = map->find(flutter::EncodableValue(key));
  if (iter != map->end() && !iter->second.IsNull()) {
    if (auto* value = std::get_if<T>(&iter->second)) {
      out = *value;
      return true;
    }
  }
  return false;
}

class AudioplayersElinuxPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar* registrar) {
    auto plugin = std::make_unique<AudioplayersElinuxPlugin>(registrar);
    {
      auto channel =
          std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "xyz.luan/audioplayers",
            &flutter::StandardMethodCodec::GetInstance());
      channel->SetMethodCallHandler(
          [plugin_pointer = plugin.get()](const auto &call, auto result) {
            plugin_pointer->HandleMethodCall(call, std::move(result));
          });
    }
    {
      auto channel =
          std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "xyz.luan/audioplayers.global",
            &flutter::StandardMethodCodec::GetInstance());
      channel->SetMethodCallHandler(
          [plugin_pointer = plugin.get()](const auto &call, auto result) {
            plugin_pointer->HandleGlobalMethodCall(call, std::move(result));
          });
    }
    registrar->AddPlugin(std::move(plugin));
  }

  AudioplayersElinuxPlugin(flutter::PluginRegistrar* registrar)
      : registrar_(registrar) {
      GstAudioPlayer::GstLibraryLoad();
  }

  virtual ~AudioplayersElinuxPlugin() {}

  void SetRegistrar(flutter::PluginRegistrar* registrar) {
    registrar_ = registrar;
  }

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    const auto* arguments =
        std::get_if<flutter::EncodableMap>(method_call.arguments());
    if (!arguments) {
      result->Error(kInvalidArgument, "No arguments provided.");
      return;
    }

    std::string player_id;
    if (!GetValueFromEncodableMap(arguments, "playerId", player_id)) {
      result->Error(kInvalidArgument, "No playerId provided.");
      return;
    }

    const std::string &method_name = method_call.method_name();
    if (method_name == "create") {
      CreateAudioPlayer(player_id);
      result->Success();
      return;
    }

    GstAudioPlayer* player = GetAudioPlayer(player_id);
    if (!player) {
      result->Error(kInvalidArgument,
                    "No AudioPlayer" + player_id + " is exist.");
      return;
    }

    if (method_name == "resume") {
      player->Resume();
      result->Success();
    } else if (method_name == "pause") {
      player->Pause();
      result->Success();
    } else if (method_name == "stop") {
      player->Stop();
      result->Success();
    } else if (method_name == "release") {
      player->Release();
      result->Success();
    } else if (method_name == "seek") {
      int32_t position = 0;
      GetValueFromEncodableMap(arguments, "position", position);
      player->Seek(position);
      result->Success();
    } else if (method_name == "setVolume") {
      double volume = 0;
      GetValueFromEncodableMap(arguments, "volume", volume);
      player->SetVolume(volume);
      result->Success();
    } else if (method_name == "setSourceUrl") {
      bool is_local = false;
      GetValueFromEncodableMap(arguments, "isLocal", is_local);
      std::string url = "";
      GetValueFromEncodableMap(arguments, "url", url);
      if (is_local) {
        url = std::string("file://") + url;
      }
      player->SetSourceUrl(url);
      result->Success();
    } else if (method_name == "setPlaybackRate") {
      double rate = 0;
      GetValueFromEncodableMap(arguments, "playbackRate", rate);
      player->SetPlaybackRate(rate);
      result->Success();
    } else if (method_name == "setReleaseMode") {
      std::string release_mode = "";
      GetValueFromEncodableMap(arguments, "releaseMode", release_mode);
      bool looping = release_mode.find("loop") != std::string::npos;
      player->SetLooping(looping);
      result->Success();
    } else if (method_name == "getDuration") {
      int64_t duration = player->GetDuration();
      if (duration >= 0) {
        result->Success(flutter::EncodableValue(duration));
      } else {
        result->Success();
      }
    } else if (method_name == "getCurrentPosition") {
      int64_t position = player->GetCurrentPosition();
      if (position >= 0) {
        result->Success(flutter::EncodableValue(position));
      } else {
        result->Success();
      }
    } else if (method_name == "setBalance") {
      double balance = 0;
      GetValueFromEncodableMap(arguments, "balance", balance);
      player->SetBalance(balance);
      result->Success();
    }
    else if (method_name == "setPlayerMode") {
      result->NotImplemented();
    } else if (method_name == "setAudioContext") {
      result->NotImplemented();
    } else if (method_name == "emitLog") {
      result->NotImplemented();
    } else if (method_name == "emitError") {
      result->NotImplemented();
    } else if (method_name == "dispose") {
      player->Dispose();
      audio_players_.erase(player_id);
      event_sinks_.erase(player_id);
      result->Success();
    } else {
      result->NotImplemented();
    }
  }

  void HandleGlobalMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    const std::string &method_name = method_call.method_name();
    if (method_name == "setAudioContext") {
      result->NotImplemented();
    } else if (method_name == "emitLog") {
      result->NotImplemented();
    } else if (method_name == "emitError") {
      result->NotImplemented();
    } else {
      result->NotImplemented();
    }
  }

  GstAudioPlayer* GetAudioPlayer(const std::string &player_id) {
    auto iter = audio_players_.find(player_id);
    if (iter != audio_players_.end()) {
      return iter->second.get();
    }
    return nullptr;
  }

  void CreateAudioPlayer(const std::string &player_id) {
    auto event_channel =
        std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
            registrar_->messenger(),
            "xyz.luan/audioplayers/events/" + player_id,
            &flutter::StandardMethodCodec::GetInstance());
    auto event_channel_handler = std::make_unique<
        flutter::StreamHandlerFunctions<flutter::EncodableValue>>(
            // StreamHandlerFunctions
            [this, id = player_id](
              const flutter::EncodableValue* arguments,
              std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&&
                  events)
              -> std::unique_ptr<
                  flutter::StreamHandlerError<flutter::EncodableValue>> {
            this->event_sinks_[id] = std::move(events);
            return nullptr;
          },
          // StreamHandlerCancel
          [](const flutter::EncodableValue* arguments)
              -> std::unique_ptr<
                  flutter::StreamHandlerError<flutter::EncodableValue>> {
            return nullptr;
          });
    event_channel->SetStreamHandler(std::move(event_channel_handler));

    auto player_handler = std::make_unique<AudioPlayerStreamHandlerImpl>(
        // OnNotifyPrepared
        [this](const std::string &player_id, bool is_prepared) {
          flutter::EncodableMap map = {
              {flutter::EncodableValue("event"),
               flutter::EncodableValue(kAudioPreparedEvent)},
              {flutter::EncodableValue("value"),
               flutter::EncodableValue(is_prepared)}};
          event_sinks_[player_id]->Success(flutter::EncodableValue(map));
        },
        // OnNotifyDuration
        [this](const std::string &player_id, int32_t duration) {
          flutter::EncodableMap map = {
              {flutter::EncodableValue("event"),
               flutter::EncodableValue(kAudioDurationEvent)},
              {flutter::EncodableValue("value"),
               flutter::EncodableValue(duration)}};
          event_sinks_[player_id]->Success(flutter::EncodableValue(map));
        },
        // OnNotifySeekCompleted
        [this](const std::string &player_id) {
          flutter::EncodableMap map = {
              {flutter::EncodableValue("event"),
               flutter::EncodableValue(kAudioSeekCompleteEvent)}};
          event_sinks_[player_id]->Success(flutter::EncodableValue(map));
        },
        // OnNotifyPlayCompleted
        [this](const std::string &player_id) {
          flutter::EncodableMap map = {
              {flutter::EncodableValue("event"),
               flutter::EncodableValue(kAudioCompleteEvent)}};
          event_sinks_[player_id]->Success(flutter::EncodableValue(map));
        },
        // OnNotifyLog
        [this](const std::string &player_id, const std::string &message) {
          flutter::EncodableMap map = {
              {flutter::EncodableValue("event"),
               flutter::EncodableValue(kAudioLogEvent)},
              {flutter::EncodableValue("value"),
               flutter::EncodableValue(message)}};
        event_sinks_[player_id]->Success(flutter::EncodableValue(map));
      });

    auto player =
        std::make_unique<GstAudioPlayer>(player_id, std::move(player_handler));
    audio_players_[player_id] = std::move(player);
  }

  std::map<std::string, std::unique_ptr<GstAudioPlayer>> audio_players_;
  std::map<std::string,
      std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>>
        event_sinks_;
  flutter::PluginRegistrar* registrar_;
};

}  // namespace

void AudioplayersElinuxPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  AudioplayersElinuxPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrar>(registrar));
}
