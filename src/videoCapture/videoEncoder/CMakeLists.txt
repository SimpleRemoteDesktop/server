project(videoCapture)

file(GLOB SRC
        *.cpp
        *.c)
if(WITH_NVENC)
	add_definitions(-DWITH_NVENC)
endif()

if(WITH_TJPEG)
	add_definitions(-DWITH_TJPEG)
endif()

add_library(videoEncoder SHARED ${SRC})
