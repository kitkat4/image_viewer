cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(image_viewer)

add_compile_options(-O2 -Wall)

set(Boost_USE_STATIC_LIBS ON)


# Decide whether to use boost or std filesystem.
find_package(Boost COMPONENTS filesystem)
if(Boost_FOUND)
  message(STATUS "Build with Boost filesystem.")
  add_definitions(-DBUILD_WITH_BOOST_FILESYSTEM)
  set(FILESYSTEM_LIBS ${Boost_LIBRARIES})
  set(CMAKE_CXX_STANDARD 11)
else()
  message(STATUS "Build with std filesystem (C++17).")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(FILESYSTEM_LIBS stdc++fs)
endif()

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
