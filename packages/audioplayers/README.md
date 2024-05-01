# audioplayers_elinux

The implementation of the Audio Player plugin for flutter elinux. APIs are designed to be API compatible with the the official [`audioplayers`](https://github.com/bluefireteam/audioplayers/tree/main/packages/audioplayers).

## Required libraries

This plugin uses [GStreamer](https://gstreamer.freedesktop.org/) internally.

```Shell
$ sudo apt install libglib2.0-dev
$ sudo apt install libgstreamer1.0-dev
# Install as needed.
$ sudo apt install libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav
```

## Usage

### pubspec.yaml
```yaml
dependencies:
  audioplayers: ^6.0.0
  audioplayers_elinux:
    git:
      url: https://github.com/sony/flutter-elinux-plugins.git
      path: packages/audioplayers
      ref: main
```

### Source code
Import `audioplayers` in your Dart code:
```dart
import 'package:audioplayers/audioplayers.dart';
```
