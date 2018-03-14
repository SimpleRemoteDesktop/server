cmake_minimum_required(VERSION 3.1)
project(SimpleRemoteDesktop_Server)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

