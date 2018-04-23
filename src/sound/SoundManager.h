//
// Created by user on 16/04/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H
#define SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H

#include <boost/thread.hpp>
#include "PulseAudioCapture.h"
#include "OpusEncoderManager.h"
#include "../encoder_ffmpeg.h"
#include "../fifo.hpp"


struct AudioPcm {
    unsigned char * data;
    int length;
};

class SoundManager {
public:
    SoundManager(Fifo<Frame> outputQueue);
    void start();
    void stop();
    void capture();

private:
    PulseAudioCapture *pulse;
    OpusEncoderManager * encoder;
    int sampleRate;
    int channels;
    int bitrate;
    bool isRunning = false;
    Fifo<Frame> outputQueue;
    Fifo<AudioPcm> *rawFifo;
};

#endif //SIMPLEREMOTEDESKTOP_SERVER_SOUNDMANAGER_H
