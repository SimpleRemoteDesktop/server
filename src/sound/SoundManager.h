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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../encoder_ffmpeg.h"
/*struct Frame { //FIXME duplicate
	unsigned char * data;
	unsigned long size;
	int type;

};*/

class SoundManager {
public:
	SoundManager(Fifo<Frame> * outputqueue);
	void start();
	void stop();
	void capture();
	Fifo<Frame> *outputqueue;

private:

	pa_simple * pa_ctx = NULL;
	OpusEncoder *encoder;
	int error;
};

#endif //SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H
