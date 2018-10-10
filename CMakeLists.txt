cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(test_code
  src/test.cpp
  src/image_viewer.cpp)

target_link_libraries(test_code
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  stdc++fs)
