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
}
