cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DIMAGE_VIEWER_WITH_X11_WINDOW)

link_directories(
  /usr/local/lib
  /usr/local/share/OpenCV/3rdparty/lib
  )

include_directories(include)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED
  Core
  Gui
  Widgets
  Test
  Concurrent)
find_package(Threads)

add_executable(image_viewer
  src/main.cpp
  src/image_viewer.cpp
  src/dir_scanner.cpp
  src/window_manager.cpp)

target_link_libraries(image_viewer
  ${OpenCV_LIBS}
  stdc++fs
  )
