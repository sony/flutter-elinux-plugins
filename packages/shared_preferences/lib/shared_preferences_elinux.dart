// Copyright 2021 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'dart:async';
import 'dart:convert' show json;

import 'package:file/file.dart';
import 'package:file/local.dart';
import 'package:meta/meta.dart';
import 'package:path/path.dart' as path;
import 'package:path_provider_elinux/path_provider_elinux.dart';
import 'package:shared_preferences_platform_interface/shared_preferences_platform_interface.dart';

/// The Linux implementation of [SharedPreferencesStorePlatform].
///
/// This class implements the `package:shared_preferences` functionality for Linux.
class SharedPreferencesELinux extends SharedPreferencesStorePlatform {
  /// The default instance of [SharedPreferencesELinux] to use.
  static SharedPreferencesELinux instance = SharedPreferencesELinux();

  /// Registers the eLinux implementation.
  static void registerWith() {
    SharedPreferencesStorePlatform.instance = instance;
  }

  /// Local copy of preferences
  Map<String, Object>? _cachedPreferences;

  /// File system used to store to disk. Exposed for testing only.
  @visibleForTesting
  FileSystem fs = LocalFileSystem();

  /// Gets the file where the preferences are stored.
  Future<File?> _getLocalDataFile() async {
    final pathProvider = PathProviderELinux();
    final directory = await pathProvider.getApplicationSupportPath();
    if (directory == null) return null;
    return fs.file(path.join(directory, 'shared_preferences.json'));
  }

  /// Gets the preferences from the stored file. Once read, the preferences are
  /// maintained in memory.
  Future<Map<String, Object>> _readPreferences() async {
    if (_cachedPreferences != null) {
      return _cachedPreferences!;
    }

    Map<String, Object> preferences = {};
    final File? localDataFile = await _getLocalDataFile();
    if (localDataFile != null && localDataFile.existsSync()) {
      String stringMap = localDataFile.readAsStringSync();
      if (stringMap.isNotEmpty) {
        preferences = json.decode(stringMap).cast<String, Object>();
      }
    }
    _cachedPreferences = preferences;
    return preferences;
  }

  /// Writes the cached preferences to disk. Returns [true] if the operation
  /// succeeded.
  Future<bool> _writePreferences(Map<String, Object> preferences) async {
    try {
      var localDataFile = await _getLocalDataFile();
      if (localDataFile == null) {
        print("Unable to determine where to write preferences.");
        return false;
      }
      if (!localDataFile.existsSync()) {
        localDataFile.createSync(recursive: true);
      }
      var stringMap = json.encode(preferences);
      localDataFile.writeAsStringSync(stringMap);
    } catch (e) {
      print("Error saving preferences to disk: $e");
      return false;
    }
    return true;
  }

  @override
  Future<bool> clear() async {
    var preferences = await _readPreferences();
    preferences.clear();
    return _writePreferences(preferences);
  }

  @override
  Future<Map<String, Object>> getAll() async {
    return _readPreferences();
  }

  @override
  Future<bool> remove(String key) async {
    var preferences = await _readPreferences();
    preferences.remove(key);
    return _writePreferences(preferences);
  }

  @override
  Future<bool> setValue(String valueType, String key, Object value) async {
    var preferences = await _readPreferences();
    preferences[key] = value;
    return _writePreferences(preferences);
  }
}
