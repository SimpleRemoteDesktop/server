
add_subdirectory(videoGrab)
add_subdirectory(videoEncoder)
file(GLOB SRC
        *.cpp
        *.c)

add_library(videoCapture videoGrab videoEncoder ${SRC})