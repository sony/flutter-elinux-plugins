// Copyright 2023 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
import 'package:flutter_test/flutter_test.dart';
import 'package:path_provider_elinux/path_provider_elinux.dart';
import 'package:path_provider_platform_interface/path_provider_platform_interface.dart';
import 'package:xdg_directories/xdg_directories.dart' as xdg;

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();
  PathProviderELinux.registerWith();

  test('registered instance', () {
    expect(PathProviderPlatform.instance, isA<PathProviderELinux>());
  });

  test('getTemporaryPath defaults to TMPDIR', () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
      environment: <String, String>{'TMPDIR': '/run/user/0/tmp'},
    );
    expect(await plugin.getTemporaryPath(), '/run/user/0/tmp');
  });

  test('getTemporaryPath uses fallback if TMPDIR is empty', () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
      environment: <String, String>{'TMPDIR': ''},
    );
    expect(await plugin.getTemporaryPath(), '/tmp');
  });

  test('getTemporaryPath uses fallback if TMPDIR is unset', () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
      environment: <String, String>{},
    );
    expect(await plugin.getTemporaryPath(), '/tmp');
  });

  test('getApplicationSupportPath', () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
        executableName: 'path_provider_elinux_test_binary',
        applicationId: 'com.example.Test');
    // Note this will fail if ${xdg.dataHome.path}/path_provider_linux_test_binary exists on the local filesystem.
    expect(await plugin.getApplicationSupportPath(),
        '${xdg.dataHome.path}/com.example.Test');
  });

  test('getApplicationSupportPath uses executable name if no application Id',
      () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
        executableName: 'path_provider_elinux_test_binary');
    expect(await plugin.getApplicationSupportPath(),
        '${xdg.dataHome.path}/path_provider_elinux_test_binary');
  });

  test('getApplicationDocumentsPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getApplicationDocumentsPath(), startsWith('/'));
  });

  test('getApplicationCachePath', () async {
    final PathProviderPlatform plugin = PathProviderELinux.private(
        executableName: 'path_provider_elinux_test_binary');
    expect(await plugin.getApplicationCachePath(),
        '${xdg.cacheHome.path}/path_provider_elinux_test_binary');
  });

  test('getDownloadsPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getDownloadsPath(), startsWith('/'));
  });
}
