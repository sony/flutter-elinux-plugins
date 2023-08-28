// Copyright 2022 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/camera_elinux/camera_elinux_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>

#include <memory>

#include "camera_stream_handler_impl.h"
#include "channels/event_channel_image_stream.h"
#include "channels/method_channel_camera.h"
#include "channels/method_channel_device.h"
#include "events/camera_initialized_event.h"
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

  CameraPlugin(flutter::PluginRegistrar* plugin_registrar,
               flutter::TextureRegistrar* texture_registrar)
      : plugin_registrar_(plugin_registrar),
        texture_registrar_(texture_registrar) {
    GstCamera::GstLibraryLoad();
  }
  virtual ~CameraPlugin() {
    if (camera_) {
      camera_->Stop();
      camera_ = nullptr;
    }
    GstCamera::GstLibraryUnload();
  }

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
  void HandleTakePictureCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleGetMinExposureOffsetCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleGetMaxExposureOffsetCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleStartImageStreamCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleStopImageStreamCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleGetMaxZoomLevelCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleGetMinZoomLevelCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleSetZoomLevelCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleLockCaptureOrientationCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void HandleDisposeCall(
      const flutter::EncodableValue* message,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  flutter::PluginRegistrar* plugin_registrar_;
  flutter::TextureRegistrar* texture_registrar_;

  std::unique_ptr<FlutterDesktopPixelBuffer> buffer_;
  std::unique_ptr<flutter::TextureVariant> texture_;
  std::unique_ptr<GstCamera> camera_ = nullptr;
  int64_t texture_id_;

  std::unique_ptr<EventChannelImageStream> event_channel_image_stream_ =
      nullptr;
  std::unique_ptr<MethodChannelCamera> method_channel_camera_;
  std::unique_ptr<MethodChannelDevice> method_channel_device_;
};

// static
void CameraPlugin::RegisterWithRegistrar(flutter::PluginRegistrar* registrar) {
  auto plugin =
      std::make_unique<CameraPlugin>(registrar, registrar->texture_registrar());
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

  if (!method_name.compare(kCameraChannelApiAvailableCameras)) {
    HandleAvailableCamerasCall(std::move(result));
  } else if (!method_name.compare(kCameraChannelApiCreate)) {
    HandleCreateCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiInitialize)) {
    HandleInitializeCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiTakePicture)) {
    HandleTakePictureCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiPrepareForVideoRecording)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiStartVideoRecording)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiStopVideoRecording)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiPauseVideoRecording)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiResumeVideoRecording)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetFlashMode)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetExposureMode)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetExposurePoint)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiGetMinExposureOffset)) {
    HandleGetMinExposureOffsetCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiGetMaxExposureOffset)) {
    HandleGetMaxExposureOffsetCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiGetExposureOffsetStepSize)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetExposureOffset)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetFocusMode)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiSetFocusPoint)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiStartImageStream)) {
    HandleStartImageStreamCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiStopImageStream)) {
    HandleStopImageStreamCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiGetMaxZoomLevel)) {
    HandleGetMaxZoomLevelCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiGetMinZoomLevel)) {
    HandleGetMinZoomLevelCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiSetZoomLevel)) {
    HandleSetZoomLevelCall(method_call.arguments(), std::move(result));
  } else if (!method_name.compare(kCameraChannelApiLockCaptureOrientation)) {
    HandleLockCaptureOrientationCall(method_call.arguments(),
                                     std::move(result));
  } else if (!method_name.compare(kCameraChannelApiUnlockCaptureOrientation)) {
    result->NotImplemented();
  } else if (!method_name.compare(kCameraChannelApiDispose)) {
    HandleDisposeCall(method_call.arguments(), std::move(result));
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
    camera.SetLensFacing("external");
    cameras.push_back(camera.ToMap());
  }

  result->Success(flutter::EncodableValue(cameras));
}

void CameraPlugin::HandleCreateCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  buffer_ = std::make_unique<FlutterDesktopPixelBuffer>();
  texture_ =
      std::make_unique<flutter::TextureVariant>(flutter::PixelBufferTexture(
          [this](size_t width,
                 size_t height) -> const FlutterDesktopPixelBuffer* {
            buffer_->width = camera_->GetPreviewWidth();
            buffer_->height = camera_->GetPreviewHeight();
            buffer_->buffer = camera_->GetPreviewFrameBuffer();

            // TODO: We need to handle this code (event_channel_image_stream_)
            // in the proper place, but the Camera plugin doesn't have a main
            // loop.
            if (event_channel_image_stream_) {
              event_channel_image_stream_->Send(buffer_->width, buffer_->height,
                                                buffer_->buffer);
            }

            return buffer_.get();
          }));
  auto texture_id = texture_registrar_->RegisterTexture(texture_.get());
  auto stream_handler =
      std::make_unique<CameraStreamHandlerImpl>([texture_id, this]() {
        texture_registrar_->MarkTextureFrameAvailable(texture_id);
      });

  camera_ = std::make_unique<GstCamera>(std::move(stream_handler));
  texture_id_ = texture_id;

  flutter::EncodableMap reply;
  reply[flutter::EncodableValue("cameraId")] =
      flutter::EncodableValue(texture_id);

  result->Success(flutter::EncodableValue(reply));
}

void CameraPlugin::HandleInitializeCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  camera_->Play();
  double preview_width = camera_->GetPreviewWidth();
  double preview_height = camera_->GetPreviewHeight();

  {
    method_channel_camera_ =
        std::make_unique<MethodChannelCamera>(plugin_registrar_, texture_id_);

    CameraInitializedEvent message;
    message.SetPreviewWidth(preview_width);
    message.SetPreviewHeight(preview_height);
    message.SetFocusMode(FocusMode::kAuto);
    message.SetExposureMode(ExposureMode::kAuto);
    message.SetFocusPointSupported(false);
    message.SetExposurePointSupported(false);

    method_channel_camera_->SendInitializedEvent(message);
  }

  {
    method_channel_device_ =
        std::make_unique<MethodChannelDevice>(plugin_registrar_);
    auto orientation = DeviceOrientation::kLandscapeRight;

    method_channel_device_->SendDeviceOrientationChangeEvent(orientation);
  }
  result->Success();
}

void CameraPlugin::HandleTakePictureCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }
  camera_->TakePicture([p_result = result.release()](
                           const std::string& captured_file_path) {
    if (!captured_file_path.empty()) {
      flutter::EncodableValue value(captured_file_path);
      p_result->Success(value);
    } else {
      p_result->Error("Failed to capture", "Failed to capture a camera image");
    }
    delete p_result;
  });
}

void CameraPlugin::HandleGetMinExposureOffsetCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }

  // TODO: Implement exposure control support
  double value = 0.0;
  result->Success(flutter::EncodableValue(value));
}

void CameraPlugin::HandleGetMaxExposureOffsetCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }

  // TODO: Implement exposure control support
  double value = 0.0;
  result->Success(flutter::EncodableValue(value));
}

void CameraPlugin::HandleStartImageStreamCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  event_channel_image_stream_ =
      std::make_unique<EventChannelImageStream>(plugin_registrar_);
  result->Success();
}

void CameraPlugin::HandleStopImageStreamCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  event_channel_image_stream_ = nullptr;
  result->Success();
}

void CameraPlugin::HandleGetMaxZoomLevelCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }
  result->Success(flutter::EncodableValue(camera_->GetMaxZoomLevel()));
}

void CameraPlugin::HandleGetMinZoomLevelCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }
  result->Success(flutter::EncodableValue(camera_->GetMinZoomLevel()));
}

void CameraPlugin::HandleSetZoomLevelCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (!camera_) {
    result->Error("Not found an active camera",
                  "Check for creating a camera device");
    return;
  }

  auto meta = ZoomLevelMessage::FromMap(*message);
  if (camera_->SetZoomLevel(meta.GetZoom())) {
    result->Success();
  } else {
    result->Error("Failed to change the zoom level", "Check the zoom level");
  }
}

void CameraPlugin::HandleLockCaptureOrientationCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  result->NotImplemented();
}

void CameraPlugin::HandleDisposeCall(
    const flutter::EncodableValue* message,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  // TODO: add multi camera support.
  if (camera_) {
    camera_->Stop();
    camera_ = nullptr;
    texture_registrar_->UnregisterTexture(texture_id_);
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
