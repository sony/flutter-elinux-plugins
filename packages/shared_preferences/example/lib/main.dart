// Copyright 2023 Sony Group Corporation. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// ignore_for_file: public_member_api_docs

import 'dart:async';

import 'package:flutter/material.dart';
import 'package:shared_preferences_elinux/shared_preferences_elinux.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      title: 'SharedPreferences Demo',
      home: SharedPreferencesDemo(),
    );
  }
}

class SharedPreferencesDemo extends StatefulWidget {
  const SharedPreferencesDemo({super.key});

  @override
  SharedPreferencesDemoState createState() => SharedPreferencesDemoState();
}

class SharedPreferencesDemoState extends State<SharedPreferencesDemo> {
  final SharedPreferencesELinux prefs = SharedPreferencesELinux();
  late Future<int> _counter;

  Future<void> _incrementCounter() async {
    final Map<String, Object> values = await prefs.getAll();
    final int counter = (values['counter'] as int? ?? 0) + 1;

    setState(() {
      _counter = prefs.setValue('Int', 'counter', counter).then((bool success) {
        return counter;
      });
    });
  }

  @override
  void initState() {
    super.initState();
    _counter = prefs.getAll().then((Map<String, Object> values) {
      return values['counter'] as int? ?? 0;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('SharedPreferences Demo'),
      ),
      body: Center(
          child: FutureBuilder<int>(
              future: _counter,
              builder: (BuildContext context, AsyncSnapshot<int> snapshot) {
                switch (snapshot.connectionState) {
                  case ConnectionState.none:
                  case ConnectionState.waiting:
                    return const CircularProgressIndicator();
                  case ConnectionState.active:
                  case ConnectionState.done:
                    if (snapshot.hasError) {
                      return Text('Error: ${snapshot.error}');
                    } else {
                      return Text(
                        'Button tapped ${snapshot.data} time${snapshot.data == 1 ? '' : 's'}.\n\n'
                        'This should persist across restarts.',
                      );
                    }
                }
              })),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: const Icon(Icons.add),
      ),
    );
  }
}
