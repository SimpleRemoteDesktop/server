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


add_subdirectory(input)
add_subdirectory(sound)
add_executable (SRDServer ${SRC} )
target_link_libraries( SRDServer input sound ${PULSEAUDIO_LIBRARY} -lpulse-simple ${Opus_LIBRARY} ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES} ${XTEST_LIBRARY} ${X11_LIBRARY} ${XEXT_LIBRARY} ${XFIXES_LIBRARY} ${Threads_LIBRARIES})
