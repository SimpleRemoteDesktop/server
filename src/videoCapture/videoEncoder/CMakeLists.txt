project(videoCapture)

file(GLOB SRC
        *.cpp
        *.c)

add_library(videoEncoder SHARED ${SRC})