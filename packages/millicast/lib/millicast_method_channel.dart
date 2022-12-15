import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'millicast_platform_interface.dart';
import 'constants.dart' as Constants;

/// An implementation of [MillicastPlatform] that uses method channels.
class MethodChannelMillicast extends MillicastPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('millicast');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<void> init() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodInit);
  }

  @override
  Future<void> printAudioSrc() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodPrintAudioSrc);
  }

  @override
  Future<void> printVideoSrc() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodPrintVideoSrc);
  }

  @override
  Future<void> setCredentials(String api_url, String stream_name, String token) async {
    await methodChannel.invokeMethod<String>(Constants.kMethodSetCredentials, {Constants.kArgsApiUrl: api_url, Constants.kArgsStreamName: stream_name, Constants.kArgsToken: token});
  }

  @override
  Future<void> setAudioSrc(String audio_src) async {
    await methodChannel.invokeMethod<String>(Constants.kMethodSetAudioSrc, {Constants.kArgsAudioSrc: audio_src});
  }

  @override
  Future<void> setVideoSrc(String video_src) async {
    await methodChannel.invokeMethod<String>(Constants.kMethodPrintVideoSrc, {Constants.kArgsVideoSrc: video_src});
  }

  @override
  Future<void> connect() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodConnect);
  }

  @override
  Future<void> printSupportedAudioCodecs() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodPrintSuppAud);
  }

  @override
  Future<void> printSupportedVideoCodecs() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodPrintSuppVid);
  }

  @override
  Future<void> setCodecs(String audio_cdc, String video_cdc) async {
    await methodChannel.invokeMethod<String>(Constants.kMethodSetCodecs, {Constants.kArgsAudioCdc: audio_cdc, Constants.kArgsVideoCdc: video_cdc});
  }

  @override
  Future<void> dispose() async {
    await methodChannel.invokeMethod<String>(Constants.kMethodDispose);
  }
}
