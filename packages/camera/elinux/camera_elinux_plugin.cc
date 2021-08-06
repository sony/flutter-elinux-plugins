// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/camera_elinux/camera_elinux_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>

#include <memory>

#include "gst_camera.h"
#include "messages/messages.h"

namespace {
constexpr char kCameraChannelName[] = "plugins.flutter.io/camera";

constexpr char kCameraChannelApiAvailableCameras[] = "availableCameras";
constexpr char kCameraChannelApiCreate[] = "create";
constexpr char kCameraChannelApiInitialize[] = "initialize";
constexpr char kCameraChannelApiTakePicture[] = "takePicture";
constexpr char kCameraChannelApiPrepareForVideoRecording[] =
    "prepareForVideoRecording";
constexpr char kCameraChannelApiStartVideoRecording[] = "startVideoRecording";
constexpr char kCameraChannelApiStopVideoRecording[] = "stopVideoRecording";
constexpr char kCameraChannelApiPauseVideoRecording[] = "pauseVideoRecording";
constexpr char kCameraChannelApiResumeVideoRecording[] = "resumeVideoRecording";
constexpr char kCameraChannelApiSetFlashMode[] = "setFlashMode";
constexpr char kCameraChannelApiSetExposureMode[] = "setExposureMode";
constexpr char kCameraChannelApiSetExposurePoint[] = "setExposurePoint";
constexpr char kCameraChannelApiGetMinExposureOffset[] = "getMinExposureOffset";
constexpr char kCameraChannelApiGetMaxExposureOffset[] = "getMaxExposureOffset";
constexpr char kCameraChannelApiGetExposureOffsetStepSize[] =
    "getExposureOffsetStepSize";
constexpr char kCameraChannelApiSetExposureOffset[] = "setExposureOffset";
constexpr char kCameraChannelApiSetFocusMode[] = "setFocusMode";
constexpr char kCameraChannelApiSetFocusPoint[] = "setFocusPoint";
constexpr char kCameraChannelApiStartImageStream[] = "startImageStream";
constexpr char kCameraChannelApiStopImageStream[] = "stopImageStream";
constexpr char kCameraChannelApiGetMaxZoomLevel[] = "getMaxZoomLevel";
constexpr char kCameraChannelApiGetMinZoomLevel[] = "getMinZoomLevel";
constexpr char kCameraChannelApiSetZoomLevel[] = "setZoomLevel";
constexpr char kCameraChannelApiLockCaptureOrientation[] =
    "lockCaptureOrientation";
constexpr char kCameraChannelApiUnlockCaptureOrientation[] =
    "unlockCaptureOrientation";
constexpr char kCameraChannelApiDispose[] = "dispose";

class CameraPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar* registrar);

  CameraPlugin(flutter::TextureRegistrar* texture_registrar)
      : texture_registrar_(texture_registrar) {}
  virtual ~CameraPlugin() {}

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  void HandleAvailableCamerasCall(
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleCreateCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleInitializeCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  flutter::TextureRegistrar* texture_registrar_;
  std::unique_ptr<GstCamera> camera_;
};

// static
void CameraPlugin::RegisterWithRegistrar(flutter::PluginRegistrar* registrar) {
  auto plugin = std::make_unique<CameraPlugin>(registrar->texture_registrar());
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), kCameraChannelName,
          &flutter::StandardMethodCodec::GetInstance());

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

void CameraPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  const std::string& method_name = method_call.method_name();

  texture_registrar_ = nullptr;
  std::cout << method_name.c_str() << " called" << std::endl;

  if (method_name.compare(kCameraChannelApiAvailableCameras) == 0) {
    HandleAvailableCamerasCall(std::move(result));
  } else if (method_name.compare(kCameraChannelApiCreate) == 0) {
    HandleCreateCall(method_call.arguments(), std::move(result));
  } else if (method_name.compare(kCameraChannelApiInitialize) == 0) {
    HandleInitializeCall(method_call.arguments(), std::move(result));
  } else if (method_name.compare(kCameraChannelApiTakePicture) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiPrepareForVideoRecording) ==
             0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiStartVideoRecording) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiStopVideoRecording) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiPauseVideoRecording) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiResumeVideoRecording) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetFlashMode) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetExposureMode) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetExposurePoint) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiGetMinExposureOffset) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiGetMaxExposureOffset) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiGetExposureOffsetStepSize) ==
             0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetExposureOffset) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetFocusMode) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetFocusPoint) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiStartImageStream) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiStopImageStream) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiGetMaxZoomLevel) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiGetMinZoomLevel) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiSetZoomLevel) == 0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiLockCaptureOrientation) ==
             0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiUnlockCaptureOrientation) ==
             0) {
    result->NotImplemented();
  } else if (method_name.compare(kCameraChannelApiDispose) == 0) {
    result->NotImplemented();
  } else {
    result->NotImplemented();
  }
}

void CameraPlugin::HandleAvailableCamerasCall(
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  flutter::EncodableList cameras;

  // TODO: add multi camera support.
  for (int i = 0; i < 1; i++) {
    AvailableCamerasMessage camera;
    camera.SetName("camera" + std::to_string(i));
    camera.SetSensorOrientation(0);
    camera.SetLensFacing("back");
    cameras.push_back(camera.ToMap());
  }

  result->Success(flutter::EncodableValue(cameras));
}

void CameraPlugin::HandleCreateCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!message) {
    return;
  }

  // auto meta = CreateMessage::FromMap(message);
  camera_ = std::make_unique<GstCamera>();
  // camera_->Play();

  flutter::EncodableMap reply;
  reply[flutter::EncodableValue("cameraId")] =
      flutter::EncodableValue((int64_t)1);  // texture id

  result->Success(flutter::EncodableValue(reply));
}

void CameraPlugin::HandleInitializeCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!message) {
    return;
  }

  result->Success();
}

}  // namespace

void CameraElinuxPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  CameraPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrar>(registrar));
}
