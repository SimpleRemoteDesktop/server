//
// Created by sylvain on 20/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_SOFTWAREENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_SOFTWAREENCODER_H

extern "C" {
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixfmt.h"
#include "libavutil/samplefmt.h"
#include <libswscale/swscale.h>
};

#include "../../Image.h"
#include "../../config.h"


class SoftwareEncoder {
public:
    SoftwareEncoder(int imageWidth, int imageHeight, int codecWidth, int codecHeight, int bit_rate, int fps, int pix_fmt_int );
    ~SoftwareEncoder();
    SRD_Buffer_Frame * encode(Image* image);

    int width;
    int height;
    AVCodec *codec;
    AVCodecContext *c;
    AVFrame *frame;
    SwsContext *sws_ctx;
    AVPacket pkt;
    int64_t i;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_SOFTWAREENCODER_H
