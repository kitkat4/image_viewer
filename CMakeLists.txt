cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(image_viewer
  src/main.cpp
  src/image_viewer.cpp)

target_link_libraries(image_viewer
  opencv_world
  # opencv_core
  # opencv_highgui
  # opencv_imgcodecs
  # opencv_imgproc
  stdc++fs)

