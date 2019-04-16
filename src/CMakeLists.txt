file(GLOB SRC
	*.cpp
	*.c)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package( Boost REQUIRED COMPONENTS thread log)
find_package (Threads)
find_package(X11)
find_package(XTest)
find_package(Xext)
find_package(XFixes)
find_package(FFmpeg)
find_package(Opus)
find_package(PulseAudio)

add_executable (SRDServer ${SRC} )
target_link_libraries( SRDServer network input sound videoCapture videoGrab videoEncoder
        ${PULSEAUDIO_LIBRARY} -lpulse-simple ${Opus_LIBRARY} ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES} ${XTEST_LIBRARY} ${X11_LIBRARY} ${XEXT_LIBRARY} ${XFIXES_LIBRARY} ${Threads_LIBRARIES})

add_subdirectory(input)
add_subdirectory(sound)
add_subdirectory(network)
add_subdirectory(videoCapture)
message("Looking for Hardware encoder")
if(WITH_NVENC)
	message("NVIDIA NVENC option enabled")
	set(NVPIPE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(NvPipe)
	target_link_libraries(SRDServer NvPipe)
endif()

install(TARGETS SRDServer DESTINATION bin)
