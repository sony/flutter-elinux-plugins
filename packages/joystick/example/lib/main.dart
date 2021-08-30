import 'dart:async';
import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';
import 'package:joystick/joystick.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  int _fd = -1;
  int _ev_time = 0;
  int _ev_value = 0;
  int _ev_type = 0;
  int _ev_number = 0;

  @override
  void initState() {
    super.initState();

    const String device = '/dev/input/js0';
    _fd = joystickOpen(device.toNativeUtf8());
    if (_fd < 0) {
      return;
    }

    Timer.periodic(
      const Duration(milliseconds: 13),
      _onPolling,
    );
  }

  void _onPolling(Timer timer) {
    final Pointer<JSEvent> pEv = malloc<JSEvent>();
    if (joystickRead(_fd, pEv) < 0) {
      return;
    }
    setState(() {
      _ev_time = pEv.ref.type;
      _ev_value = pEv.ref.value;
      _ev_type = pEv.ref.type;
      _ev_number = pEv.ref.number;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Joystick example app'),
        ),
        body: Center(
          child: Text('Joystick event: \n'
              'time = $_ev_time\n'
              'value = $_ev_value\n'
              'type = $_ev_type\n'
              'number = $_ev_number\n'),
        ),
      ),
    );
  }
}
