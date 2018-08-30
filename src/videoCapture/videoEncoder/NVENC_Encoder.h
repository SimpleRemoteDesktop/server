//
// Created by sylvain on 29/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H


#include "../../config.h"
#include "../../Image.h"
#include "../../NvPipe/include/NvPipe.h"

class NVENC_Encoder {
public:
    NVENC_Encoder(int width, int height, int bit_rate, int fps);
    SRD_Buffer_Frame* encode(Image* image);

private:
    NvPipe *encoder;
    int width;
    int height;
    int bit_rate;
    int fps;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
