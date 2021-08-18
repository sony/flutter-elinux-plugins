cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "camera_elinux")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "camera_elinux_plugin")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(PkgConfig)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)

add_library(${PLUGIN_NAME} SHARED
  "camera_elinux_plugin.cc"
  "channels/event_channel_image_stream.cc"
  "channels/method_channel_camera.cc"
  "channels/method_channel_device.cc"
  "gst_camera.cc"
  "types/exposure_mode.cc"
  "types/focus_mode.cc"
  "types/orientation.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GStreamer)

# List of absolute paths to libraries that should be bundled with the plugin
set(camera_elinux_bundled_libraries
  ""
  PARENT_SCOPE
)
