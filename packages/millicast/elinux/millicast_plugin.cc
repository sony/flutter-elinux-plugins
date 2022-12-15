#include "include/millicast/millicast_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <algorithm>

#include <millicast-sdk/media.h>
#include <millicast-sdk/publisher.h>

namespace {

// Methods
constexpr char kMethodCreate[] = "create";
constexpr char kMethodPrintAudioSrc[] = "printAudioSrc";
constexpr char kMethodPrintVideoSrc[] = "printVideoSrc";
constexpr char kMethodSetСredentials[] = "setCredentials";
constexpr char kMethodSetAudioSrc[] = "setAudioSrc";
constexpr char kMethodSetVideoSrc[] = "setVideoSrc";
constexpr char kMethodConnect[] = "connect";
constexpr char kMethodPrintSuppAud[] = "printSupportedAudioCodecs";
constexpr char kMethodPrintSuppVid[] = "printSupportedVideoCodecs";
constexpr char kMethodSetCodecs[] = "setCodecs";
constexpr char kMethodDispose[] = "dispose";

// Arguments
constexpr char kArgsApiUrl[] = "api_url";
constexpr char kArgsStreamName[] = "stream_name";
constexpr char kArgsToken[] = "token";
constexpr char kArgsAudioSrc[] = "audio_src";
constexpr char kArgsVideoSrc[] = "video_src";
constexpr char kArgsAudioCdc[] = "audio_cdc";
constexpr char kArgsVideoCdc[] = "video_cdc";

class PubListener : public millicast::Publisher::Listener
{
  public:
  PubListener(millicast::Publisher * publisher)
  : m_publisher( publisher )
  {}
  virtual ~PubListener() = default;

  void on_connected() override { 
    if ( m_publisher )
      m_publisher->publish(); 
  }
  void on_connection_error(int, const std::string &) override {}
  void on_signaling_error(const std::string &) override {}
  void on_publishing() override {}
  void on_publishing_error(const std::string &) override {}
  void on_stats_report(const millicast::StatsReport &) override {}
  void on_active() override {}
  void on_inactive() override {}
  void on_viewer_count(int) override {}

  private:
  millicast::Publisher * m_publisher;
};

class MillicastPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

  MillicastPlugin();

  virtual ~MillicastPlugin();

 private:
  std::unique_ptr < millicast::Publisher > publisher;
  std::unique_ptr < PubListener > listener;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void MillicastPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "millicast",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<MillicastPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

MillicastPlugin::MillicastPlugin() {}

MillicastPlugin::~MillicastPlugin() {}

void MillicastPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "eLinux";
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else if (method_call.method_name().compare(kMethodCreate) == 0) {
    publisher = millicast::Publisher::create();
    listener = std::make_unique<PubListener>(publisher.get());

    publisher->set_listener(listener.get());
    
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodPrintAudioSrc) == 0) {
    // auto audio_src = millicast::Media::get_audio_sources();
    // std::cout << "Audio sources : " << std::endl;
    // for (const auto & src : audio_src)
    //   std::cout << src->name() << std::endl;
    
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodPrintVideoSrc) == 0) {
    // auto video_src = millicast::Media::get_video_sources();
    // std::cout << "Video sources : " << std::endl;
    // for (const auto & src : video_src)
    //   std::cout << src->name() << std::endl;
    
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodSetСredentials) == 0) {
    if ( !method_call.arguments() ) {
      result->Error("Argument error","No arguments were provided to set credentials call");
      return;
    }

    const auto& arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto api_url_iter = arguments.find(flutter::EncodableValue(std::string(kArgsApiUrl)));
    if (api_url_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument api_url");
      return;
    }

    auto stream_name_iter = arguments.find(flutter::EncodableValue(std::string(kArgsStreamName)));
    if (stream_name_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument stream_name");
      return;
    }

    auto token_iter = arguments.find(flutter::EncodableValue(std::string(kArgsToken)));
    if (token_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument token");
      return;
    }

    auto creds = publisher->get_credentials();
    creds.api_url = std::get<std::string>(api_url_iter->second);
    creds.stream_name = std::get<std::string>(stream_name_iter->second);
    creds.token = std::get<std::string>(token_iter->second);

    bool r = publisher->set_credentials(std::move(creds));
    
    result->Success(flutter::EncodableValue(r));
  } else if (method_call.method_name().compare(kMethodSetAudioSrc) == 0) {
    if ( !method_call.arguments() ) {
      result->Error("Argument error","No arguments were provided to set credentials call");
      return;
    }
    
    const auto& arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto audio_src = millicast::Media::get_audio_sources();
    auto audio_iter = arguments.find(flutter::EncodableValue(std::string(kArgsAudioSrc)));
    if (audio_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument token");
      return;
    }

    // auto selected_iter = std::find_if(audio_src.begin(), audio_src.begin(), 
    // [&](const millicast::AudioSource::Ptr src) -> bool
    // { return src->name() == std::get<std::string>(audio_iter->second); });

    // if ( selected_iter != audio_src.end() ) {
      // auto audio_track = (*selected_iter)->start_capture();
    if ( audio_src[0] ) {
      auto audio_track = audio_src[0]->start_capture();
      publisher->add_track(audio_track);
    } else {
      result->Error("Argument error",
                    "Invalid audio source");
      return;
    }

    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodSetVideoSrc) == 0) {
    if ( !method_call.arguments() ) {
      result->Error("Argument error","No arguments were provided to set credentials call");
      return;
    }
    
    const auto& arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto video_src = millicast::Media::get_video_sources();
    auto video_iter = arguments.find(flutter::EncodableValue(std::string(kArgsVideoSrc)));
    if (video_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument token");
      return;
    }

    // auto selected_iter = std::find_if(video_src.begin(), video_src.begin(), 
    // [&](const millicast::VideoSource::Ptr src) -> bool
    // { return src->name() == std::get<std::string>(video_iter->second); });

    // if ( selected_iter != video_src.end() ) {
      // auto video_track = (*selected_iter)->start_capture();
    if ( video_src[0] ) {
      auto video_track = video_src[0]->start_capture();
      publisher->add_track(video_track);
    } else {
      result->Error("Argument error",
                    "Invalid video source");
      return;
    }
    
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodPrintSuppAud) == 0) {
    // auto audio_codecs = millicast::Client::get_supported_audio_codecs();
    
    // std::cout << "Audio codecs : " << std::endl;
    // for (const auto & codecs : audio_codecs)
    //   std::cout << codecs << std::endl;
    
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodPrintSuppVid) == 0) {
    // auto video_codecs = millicast::Client::get_supported_video_codecs();
    
    // std::cout << "Video codecs : " << std::endl;
    // for (const auto & codecs : video_codecs)
    //   std::cout << codecs << std::endl;

    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodSetCodecs) == 0) {
    if ( !method_call.arguments() ) {
      result->Error("Argument error","No arguments were provided to set credentials call");
      return;
    }

    const auto& arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto audio_iter = arguments.find(flutter::EncodableValue(std::string(kArgsAudioCdc)));
    if (audio_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument audio_cdc");
      return;
    }

    auto video_iter = arguments.find(flutter::EncodableValue(std::string(kArgsVideoCdc)));
    if (video_iter == arguments.end()) {
      result->Error("Argument error",
                    "Missing argument video_cdc");
      return;
    }
    millicast::Publisher::Option options;
    
    // auto audio_codecs = millicast::Client::get_supported_audio_codecs();
    auto audio_codec_str = std::get<std::string>(audio_iter->second);
    // auto audFindIter = std::find(audio_codecs.begin(), audio_codecs.end(), audio_codec_str);
    // if (audFindIter != audio_codecs.end())
      options.codecs.audio = audio_codec_str;
    // else {
    //   result->Error("Argument error",
    //                 "Invalid audio_cdc argument provided");
    //   return;
    // }

    // auto video_codecs = millicast::Client::get_supported_video_codecs();
    auto video_codec_str = std::get<std::string>(video_iter->second);
    // auto vidFindIter = std::find(video_codecs.begin(), video_codecs.end(), video_codec_str);
    // if (vidFindIter != video_codecs.end() )
      options.codecs.video = video_codec_str;
    // else {
    //   result->Error("Argument error",
    //                 "Invalid video_cdc argument provided");
    //   return;
    // }

    publisher->set_options(options);

    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodConnect) == 0) {
    publisher->connect();
    result->Success(flutter::EncodableValue());
  } else if (method_call.method_name().compare(kMethodDispose) == 0) {
    publisher->disconnect();
    publisher.reset();
    listener.reset();
    result->Success(flutter::EncodableValue());
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void MillicastPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  MillicastPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrar>(registrar));
}
