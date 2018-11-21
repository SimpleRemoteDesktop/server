//
// Created by user on 16/04/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H
#define SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H

#include <boost/thread.hpp>
#include "../fifo.hpp"
#include <opus/opus.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../config.h"

class SoundManager {
public:
	SoundManager(Fifo<SRD_Buffer_Frame> * outputqueue);
	boost::thread start();
	void stop();
	void capture();
	Fifo<SRD_Buffer_Frame> *outputqueue;

private:

	pa_simple * pa_ctx = NULL;
	OpusEncoder *encoder;
	int error;
	bool isRunning;
};

#endif //SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H
