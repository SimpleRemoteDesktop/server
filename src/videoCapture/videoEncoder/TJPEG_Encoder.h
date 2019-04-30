//
// Created by sylvain on 29/08/18.
//
#ifdef WITH_TJPEG
#ifndef SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
extern "C" {
#include <turbojpeg.h>
};

#include "../../config.h"
#include "../../Image.h"
#include "AbstractEncoder.h"

class TJPEG_Encoder: public AbstractEncoder {
public:
    TJPEG_Encoder(int width, int height, int codecWidth, int codecHeight);
    SRD_Buffer_Frame* encode(Image* image);

private:
    tjhandle encoder;
    int width;
    int height;
    int codecWidth;
    int codecHeight;
    bool isScale;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
#endif
