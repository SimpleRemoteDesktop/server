//
// Created by user on 16/04/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H

#include <opus/opus.h>
#include "../encoder_ffmpeg.h"

class OpusEncoderManager {
public:
    OpusEncoderManager(int sampleRate, int channels, int bitrate);
    void encode(unsigned char * in, Frame* frame);
private:
    OpusEncoder *encoder;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H
