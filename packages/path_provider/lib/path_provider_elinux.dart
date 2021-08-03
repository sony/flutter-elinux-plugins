// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:path_provider_platform_interface/path_provider_platform_interface.dart';
import 'package:xdg_directories/xdg_directories.dart' as xdg;

/// The elinux implementation of [PathProviderPlatform]
///
/// This class implements the `package:path_provider` functionality for eLinux
class PathProviderELinux extends PathProviderPlatform {
  /// Registers this class as the default instance of [PathProviderPlatform]
  static void registerWith() {
    PathProviderPlatform.instance = PathProviderELinux();
  }

  @override
  Future<String?> getTemporaryPath() {
    return Future<String?>.value('/tmp');
  }

  @override
  Future<String?> getApplicationSupportPath() async {
    final String processName = path.basenameWithoutExtension(
        await File('/proc/self/exe').resolveSymbolicLinks());
    final Directory directory =
        Directory(path.join(xdg.dataHome.path, processName));
    // Creating the directory if it doesn't exist, because mobile implementations assume the directory exists
    if (!directory.existsSync()) {
      await directory.create(recursive: true);
    }
    return directory.path;
  }

  @override
  Future<String?> getApplicationDocumentsPath() {
    return Future<String?>.value(xdg.getUserDirectory('DOCUMENTS')?.path);
  }

  @override
  Future<String?> getDownloadsPath() {
    return Future<String?>.value(xdg.getUserDirectory('DOWNLOAD')?.path);
  }
}
