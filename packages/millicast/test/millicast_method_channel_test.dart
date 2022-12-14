import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:millicast/millicast_method_channel.dart';

void main() {
  MethodChannelMillicast platform = MethodChannelMillicast();
  const MethodChannel channel = MethodChannel('millicast');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
