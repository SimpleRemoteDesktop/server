//
// Created by user on 16/04/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_PULSEAUDIOCAPTURE_H
#define SIMPLEREMOTEDESKTOP_SERVER_PULSEAUDIOCAPTURE_H

#include <pulse/simple.h>
#include <pulse/error.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>

#define	PULSEAUDIO_CHUNKSIZE	1024
//#define	PULSEAUDIO_CHUNKSIZE	1024
#define AUDIOBUF_BUFSIZE	16384

class PulseAudioCapture {
public:
    PulseAudioCapture(int sampleRate, int channels);
    ~PulseAudioCapture();
    void getBuffer(unsigned char* buffer);

private:
    const char *dev;
    pa_simple * pa_ctx = NULL;
    int error;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_PULSEAUDIOCAPTURE_H
