# camera_elinux

The implementation of the camera plugin for flutter elinux. APIs are designed to be API compatible with the the official [`camera`](https://github.com/flutter/plugins/tree/master/packages/camera).

## Required libraries

This plugin uses [GStreamer](https://gstreamer.freedesktop.org/) internally.

```Shell
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
  camera: ^0.10.5+3
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

### Customization for your target devices

To improve the performance of this plugin, you will need to customize the pipeline in the source file. Please modify the source file and replace the `videoconvert` element with a H/W accelerated element of your target device to perform well.

`bool GstCamera::CreatePipeline()` in packages/camera/elinux/gst_camera.cc

#### default:

```
camerabin viewfinder-sink="videoconvert ! video/x-raw,format=RGBA ! fakesink"
```

#### i.MX 8M platforms:

```
camerabin viewfinder-sink="imxvideoconvert_g2d ! video/x-raw,format=RGBA ! fakesink"
```

## Troubleshooting

If you get the following error:
```Shell
Wrong JPEG library version: library is 62, caller expects 80
```

, try the following:
```Shell
sudo mv /usr/lib/x86_64-linux-gnu/gstreamer-1.0/libgstjpeg.so \
        /usr/lib/x86_64-linux-gnu/gstreamer-1.0/libgstjpeg.so.org
```
