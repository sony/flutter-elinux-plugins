# camera_elinux

The implementation of the camera plugin for flutter elinux. APIs are designed to be API compatible with the the official [`camera`](https://github.com/flutter/plugins/tree/master/packages/camera).

## Required libraries

This plugin uses [GStreamer](https://gstreamer.freedesktop.org/) internally.

```Shell
$ sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-0 \
    gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav
```

## Usage

### pubspec.yaml
```yaml
dependencies:
  camera: ^0.8.1+7
  camera_elinux:
    git:
      url: https://github.com/sony/flutter-elinux-plugins.git
      path: packages/camera
      ref: main
```

### Source code
Import `camera` in your Dart code:
```dart
import 'package:camera/camera.dart';
```
