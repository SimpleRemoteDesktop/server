//
// Created by user on 16/04/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H

#include <opus/opus.h>
#include "../encoder_ffmpeg.h"

class OpusEncoder {
public:
    OpusEncoder(int sampleRate, int channels, int bitrate);
    void encode(short *in, Frame* frame);
private:
    OpusEncoder *encoder;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_OPUSENCODER_H
