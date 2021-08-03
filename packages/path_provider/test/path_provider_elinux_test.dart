// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
import 'package:flutter_test/flutter_test.dart';
import 'package:path_provider_elinux/path_provider_elinux.dart';
import 'package:path_provider_platform_interface/path_provider_platform_interface.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();
  PathProviderELinux.registerWith();

  test('registered instance', () {
    expect(PathProviderPlatform.instance, isA<PathProviderELinux>());
  });

  test('getTemporaryPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getTemporaryPath(), '/tmp');
  });

  test('getApplicationSupportPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getApplicationSupportPath(), startsWith('/'));
  });

  test('getApplicationDocumentsPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getApplicationDocumentsPath(), startsWith('/'));
  });

  test('getDownloadsPath', () async {
    final PathProviderPlatform plugin = PathProviderPlatform.instance;
    expect(await plugin.getDownloadsPath(), startsWith('/'));
  });
}
