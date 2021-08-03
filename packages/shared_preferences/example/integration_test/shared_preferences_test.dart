// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'dart:async';

import 'package:flutter_test/flutter_test.dart';
import 'package:integration_test/integration_test.dart';
import 'package:shared_preferences_elinux/shared_preferences_elinux.dart';

void main() {
  IntegrationTestWidgetsFlutterBinding.ensureInitialized();

  group('SharedPreferencesELinux', () {
    const Map<String, dynamic> kTestValues = <String, dynamic>{
      'flutter.String': 'hello world',
      'flutter.bool': true,
      'flutter.int': 42,
      'flutter.double': 3.14159,
      'flutter.List': <String>['foo', 'bar'],
    };

    const Map<String, dynamic> kTestValues2 = <String, dynamic>{
      'flutter.String': 'goodbye world',
      'flutter.bool': false,
      'flutter.int': 1337,
      'flutter.double': 2.71828,
      'flutter.List': <String>['baz', 'quox'],
    };

    late SharedPreferencesELinux preferences;

    setUp(() async {
      preferences = SharedPreferencesELinux.instance;
    });

    tearDown(() {
      preferences.clear();
    });

    testWidgets('reading', (WidgetTester _) async {
      final all = await preferences.getAll();
      expect(all['String'], isNull);
      expect(all['bool'], isNull);
      expect(all['int'], isNull);
      expect(all['double'], isNull);
      expect(all['List'], isNull);
    });

    testWidgets('writing', (WidgetTester _) async {
      await Future.wait(<Future<bool>>[
        preferences.setValue(
            'String', 'String', kTestValues2['flutter.String']),
        preferences.setValue('Bool', 'bool', kTestValues2['flutter.bool']),
        preferences.setValue('Int', 'int', kTestValues2['flutter.int']),
        preferences.setValue(
            'Double', 'double', kTestValues2['flutter.double']),
        preferences.setValue('StringList', 'List', kTestValues2['flutter.List'])
      ]);
      final all = await preferences.getAll();
      expect(all['String'], kTestValues2['flutter.String']);
      expect(all['bool'], kTestValues2['flutter.bool']);
      expect(all['int'], kTestValues2['flutter.int']);
      expect(all['double'], kTestValues2['flutter.double']);
      expect(all['List'], kTestValues2['flutter.List']);
    });

    testWidgets('removing', (WidgetTester _) async {
      const String key = 'testKey';

      await Future.wait([
        preferences.setValue('String', key, kTestValues['flutter.String']),
        preferences.setValue('Bool', key, kTestValues['flutter.bool']),
        preferences.setValue('Int', key, kTestValues['flutter.int']),
        preferences.setValue('Double', key, kTestValues['flutter.double']),
        preferences.setValue('StringList', key, kTestValues['flutter.List'])
      ]);
      await preferences.remove(key);
      final all = await preferences.getAll();
      expect(all['testKey'], isNull);
    });

    testWidgets('clearing', (WidgetTester _) async {
      await Future.wait(<Future<bool>>[
        preferences.setValue('String', 'String', kTestValues['flutter.String']),
        preferences.setValue('Bool', 'bool', kTestValues['flutter.bool']),
        preferences.setValue('Int', 'int', kTestValues['flutter.int']),
        preferences.setValue('Double', 'double', kTestValues['flutter.double']),
        preferences.setValue('StringList', 'List', kTestValues['flutter.List'])
      ]);
      await preferences.clear();
      final all = await preferences.getAll();
      expect(all['String'], null);
      expect(all['bool'], null);
      expect(all['int'], null);
      expect(all['double'], null);
      expect(all['List'], null);
    });
  });
}
