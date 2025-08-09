cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(image_viewer)

add_compile_options(-O2 -Wall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FILESYSTEM_LIBS stdc++fs)

include_directories(include)
find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)

add_executable(image_viewer
  src/main.cpp
  src/image_viewer.cpp
  src/dir_scanner.cpp
  src/window_manager.cpp)

target_link_libraries(image_viewer
  pthread
  ${OpenCV_LIBS}
  ${X11_LIBRARIES}
  ${FILESYSTEM_LIBS}
  )
