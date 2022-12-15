// You have generated a new plugin project without specifying the `--platforms`
// flag. A plugin project with no platform support was generated. To add a
// platform, run `flutter create -t plugin --platforms <platforms> .` under the
// same directory. You can also find a detailed instruction on how to add
// platforms in the `pubspec.yaml` at
// https://flutter.dev/docs/development/packages-and-plugins/developing-packages#plugin-platforms.

import 'millicast_platform_interface.dart';

class Millicast {
  Future<String?> getPlatformVersion() {
    return MillicastPlatform.instance.getPlatformVersion();
  }

  Future<void> init() async {
    await MillicastPlatform.instance.init();
  }

  Future<void> printAudioSrc() async {
    await MillicastPlatform.instance.printAudioSrc();
  }

  Future<void> printVideoSrc() async {
    await MillicastPlatform.instance.printVideoSrc();
  }

  Future<void> setCredentials(
      String api_url, String stream_name, String token) async {
    await MillicastPlatform.instance
        .setCredentials(api_url, stream_name, token);
  }

  Future<void> setAudioSrc(String audio_src) async {
    await MillicastPlatform.instance.setAudioSrc(audio_src);
  }

  Future<void> setVideoSrc(String video_src) async {
    await MillicastPlatform.instance.setVideoSrc(video_src);
  }

  Future<void> connect() async {
    await MillicastPlatform.instance.connect();
  }

  Future<void> printSupportedAudioCodecs() async {
    await MillicastPlatform.instance.printSupportedAudioCodecs();
  }

  Future<void> printSupportedVideoCodecs() async {
    await MillicastPlatform.instance.printSupportedVideoCodecs();
  }

  Future<void> setCodecs(String audio_cdc, String video_cdc) async {
    await MillicastPlatform.instance.setCodecs(audio_cdc, video_cdc);
  }

  Future<void> dispose() async {
    await MillicastPlatform.instance.dispose();
  }
}
