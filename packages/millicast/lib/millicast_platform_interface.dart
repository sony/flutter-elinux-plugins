import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'millicast_method_channel.dart';

abstract class MillicastPlatform extends PlatformInterface {
  /// Constructs a MillicastPlatform.
  MillicastPlatform() : super(token: _token);

  static final Object _token = Object();

  static MillicastPlatform _instance = MethodChannelMillicast();

  /// The default instance of [MillicastPlatform] to use.
  ///
  /// Defaults to [MethodChannelMillicast].
  static MillicastPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [MillicastPlatform] when
  /// they register themselves.
  static set instance(MillicastPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<void> init() async {
    throw UnimplementedError('init() has not been implemented.');
  }

  Future<void> printAudioSrc() async {
    throw UnimplementedError('printAudioSrc() has not been implemented.');
  }

  Future<void> printVideoSrc() async {
    throw UnimplementedError('printVideoSrc() has not been implemented.');
  }

  Future<void> setCredentials(
      String api_url, String stream_name, String token) async {
    throw UnimplementedError(
        'setCredentials(String api_url, String stream_name, String token) has not been implemented.');
  }

  Future<void> setAudioSrc(String audio_src) async {
    throw UnimplementedError(
        'setAudioSrc(String audio_src) has not been implemented.');
  }

  Future<void> setVideoSrc(String video_src) async {
    throw UnimplementedError(
        'setVideoSrc(String video_src) has not been implemented.');
  }

  Future<void> connect() async {
    throw UnimplementedError('connect() has not been implemented.');
  }

  Future<void> printSupportedAudioCodecs() async {
    throw UnimplementedError(
        'printSupportedAudioCodecs() has not been implemented.');
  }

  Future<void> printSupportedVideoCodecs() async {
    throw UnimplementedError(
        'printSupportedVideoCodecs() has not been implemented.');
  }

  Future<void> setCodecs(String audio_cdc, String video_cdc) async {
    throw UnimplementedError(
        'setCodecs(String audio_cdc, String video_cdc) has not been implemented.');
  }

  Future<void> dispose() async {
    throw UnimplementedError('dispose() has not been implemented.');
  }
}
