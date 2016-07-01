# TODO: make a debug option & build lws in debugmode
cmake_minimum_required(VERSION 3.0)
project(nmq)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
include(ExternalProject)

set(DEPS_INSTALL_DIR "${CMAKE_BINARY_DIR}/usr" CACHE PATH "Dependencies install directory")
set(DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/third-party" CACHE PATH "Dependencies build directory")

add_subdirectory(third-party)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${DEPS_INSTALL_DIR}/include")
link_directories("${DEPS_INSTALL_DIR}/lib")

file(GLOB_RECURSE NMQ_SOURCES "src/*.c")

add_executable(nmq ${NMQ_SOURCES})
add_dependencies(nmq libwebsockets)
#target_link_libraries(nmq libwebsockets.so)
target_link_libraries(nmq libwebsockets.a)
