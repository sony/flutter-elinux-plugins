cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "millicast")
project(${PROJECT_NAME} LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)

set(PLUGIN_NAME "millicast_plugin")

add_library(${PLUGIN_NAME} SHARED
  "millicast_plugin.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
                      CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE 
                          "${CMAKE_CURRENT_SOURCE_DIR}/include")

# set(MillicastSDK_DIR "/usr/lib/x86_64-linux-gnu/millicast-sdk/cmake")
# find_package( MillicastSDK REQUIRED )
# target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin Millicast::MillicastSDK )

set(MillicastSDK_DIR "/usr/lib/x86_64-linux-gnu/millicast-sdk")
target_link_libraries(${PLUGIN_NAME} PRIVATE "${MillicastSDK_DIR}/libMillicastSDK.so" "${MillicastSDK_DIR}/libndi.so.5.1.1")
# add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

# List of absolute paths to libraries that should be bundled with the plugin
set(millicast_bundled_libraries
  ""
  PARENT_SCOPE
)
