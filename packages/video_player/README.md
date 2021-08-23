# video_player_elinux

The implementation of the Video Player plugin for flutter elinux. APIs are designed to be API compatible with the the official [`video_player`](https://github.com/flutter/plugins/tree/master/packages/video_player).

![image](https://user-images.githubusercontent.com/62131389/124210378-43f06400-db26-11eb-8723-40dad0eb67b0.png)

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
  video_player: ^2.1.12
  video_player_elinux:
    git:
      url: https://github.com/sony/flutter-elinux-plugins.git
      path: packages/video_player
      ref: main
```

### Source code
Import `video_player` in your Dart code:
```dart
import 'package:video_player/video_player.dart';
```

### Customize for your target devices
If you this plugin on your target devices, you will need to customize the pipeline in the source file.So, replace the `videoconvert` element with a H/W accelerated element of your target device to perform well.

#### default:
playbin uri=<file> video-sink="videoconvert ! video/x-raw,format=RGBA ! fakesink"

#### e.g. customization for i.MX 8M platforms:
playbin uri=<file> video-sink="imxvideoconvert_g2d ! video/x-raw,format=RGBA ! fakesink"
