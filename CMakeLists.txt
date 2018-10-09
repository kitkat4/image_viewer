cmake_minimum_required(VERSION 2.8)

add_executable(test_code src/test.cpp)

target_link_libraries(test_code
  # opencv_world)
  opencv_core
  opencv_highgui
  opencv_imgcodecs)

