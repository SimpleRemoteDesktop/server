cmake_minimum_required(VERSION 3.1)
project(SimpleRemoteDesktop_Server)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS, " -lXtst -lX11 -lXfixes -lavutil -lavcodec -lpthread -lswscale")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

