// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// ignore_for_file: public_member_api_docs

import 'dart:ffi';
import 'package:ffi/ffi.dart';

DynamicLibrary _dylib = DynamicLibrary.open('libjoystick_plugin.so');

/// See: <linux/joystick.h>
/// struct js_event {
/// 	__u32 time;	/* event timestamp in milliseconds */
/// 	__s16 value;	/* value */
/// 	__u8 type;	/* event type */
/// 	__u8 number;	/* axis/button number */
/// };
class JSEvent extends Struct {
  @Uint32()
  external int time;
  @Int16()
  external int value;
  @Uint8()
  external int type;
  @Uint8()
  external int number;
}

/// button pressed/released. See [JS_EVENT_BUTTON] in <linux/joystick.h>
const int JS_EVENT_BUTTON = 0x01;

/// joystick moved. See [JS_EVENT_AXIS] in <linux/joystick.h>
const int JS_EVENT_AXIS = 0x02;

/// initial state of device. See [JS_EVENT_INIT] in <linux/joystick.h>
const int JS_EVENT_INIT = 0x80;

typedef JoystickOpenNative = Int32 Function(Pointer<Utf8>);
typedef JoystickOpen = int Function(Pointer<Utf8>);

/// Opens joystick device.
final JoystickOpen joystickOpen = _dylib
    .lookup<NativeFunction<JoystickOpenNative>>('joystick_open')
    .asFunction();

typedef JoystickReadNative = Int32 Function(Int32 fd, Pointer<JSEvent>);
typedef JoystickRead = int Function(int fd, Pointer<JSEvent>);

/// Reads joystick input data.
final JoystickRead joystickRead = _dylib
    .lookup<NativeFunction<JoystickReadNative>>('joystick_read')
    .asFunction();

/// Returns true if no events.
bool joystickInputIsInactive(JSEvent ev) {
  return (ev.type & JS_EVENT_INIT) != 0;
}

/// Returns true if the event was caused by a button press.
bool joystickInputIsButton(JSEvent ev) {
  return (ev.type & JS_EVENT_BUTTON) != 0;
}

/// Returns true if the event was caused by an axis movement.
bool joystickInputIsAxis(JSEvent ev) {
  return (ev.type & JS_EVENT_AXIS) != 0;
}

/// Returns true if the button is pressed.
bool joystickButtonIsPressed(JSEvent ev) {
  return (ev.value & 1) != 0;
}
