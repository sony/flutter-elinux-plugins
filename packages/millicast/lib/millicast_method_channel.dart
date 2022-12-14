import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'millicast_platform_interface.dart';

/// An implementation of [MillicastPlatform] that uses method channels.
class MethodChannelMillicast extends MillicastPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('millicast');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
