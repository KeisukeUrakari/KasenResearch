cmake_minimum_required(VERSION 3.10)

set(Project app)

project(${Project} CXX C)

set(libs)
set(ext-libs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/c.cmake)
include(cmake/cxx.cmake)
include(cmake/gtest.cmake)
include(cmake/png.cmake)
include(cmake/freetype.cmake)
include(cmake/iconv.cmake)
#include(cmake/qt.cmake)
#include(cmake/boost.cmake)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(mosquitto)
add_subdirectory(newt)
add_subdirectory(beast)
add_subdirectory(multiio)
