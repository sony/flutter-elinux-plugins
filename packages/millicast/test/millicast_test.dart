import 'package:flutter_test/flutter_test.dart';
import 'package:millicast/millicast.dart';
import 'package:millicast/millicast_platform_interface.dart';
import 'package:millicast/millicast_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockMillicastPlatform
    with MockPlatformInterfaceMixin
    implements MillicastPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final MillicastPlatform initialPlatform = MillicastPlatform.instance;

  test('$MethodChannelMillicast is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelMillicast>());
  });

  test('getPlatformVersion', () async {
    Millicast millicastPlugin = Millicast();
    MockMillicastPlatform fakePlatform = MockMillicastPlatform();
    MillicastPlatform.instance = fakePlatform;

    expect(await millicastPlugin.getPlatformVersion(), '42');
  });
}
