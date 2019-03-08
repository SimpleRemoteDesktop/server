//
// Created by sylvain on 29/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
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

#include "../../config.h"
#include "../../Image.h"
#include "../../NvPipe/include/NvPipe.h"
#include "AbstractEncoder.h"

class NVENC_Encoder: public AbstractEncoder {
public:
    NVENC_Encoder(int width, int height, int codecWidth, int codecHeight, int bit_rate, int fps);
    SRD_Buffer_Frame* encode(Image* image);

private:
    NvPipe *encoder;
    int width;
    int height;
    int bit_rate;
    int fps;
    int codecWidth;
    int codecHeight;
    int interval;
    bool isScale;
    SwsContext *sws_ctx;
    AVFrame *frame;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NVENC_ENCODER_H
