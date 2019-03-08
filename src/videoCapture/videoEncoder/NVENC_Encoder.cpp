//
// Created by sylvain on 29/08/18.
//
#ifdef WITH_NVENC
#include <vector>
#include <cstring>
#include "NVENC_Encoder.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

NVENC_Encoder::NVENC_Encoder(int width, int height, int codecWidth, int codecHeight, int bit_rate, int fps) {
    BOOST_LOG_TRIVIAL(info) << " starting NVENC  encoder";
    this->width = width;
    this->height = height;
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;
    this->bit_rate = bit_rate;
    this->fps = fps;
    this->interval = 0;
    this->isScale = !((this->width == this->codecWidth) && (this->height == this->codecHeight));

    if(this->isScale) {
        BOOST_LOG_TRIVIAL(info) << "NVENC scaling image enable";
        this->frame = av_frame_alloc();
        if (!this->frame) {
            BOOST_LOG_TRIVIAL(error) << "Could not allocate video frame\n";
        }
        this->frame->format =   AV_PIX_FMT_RGBA;
        this->frame->width  = this->codecWidth;
        this->frame->height = this->codecHeight;


        int ret = av_image_alloc(frame->data, frame->linesize, this->codecWidth, this->codecHeight,AV_PIX_FMT_RGBA, 32);
        if (ret < 0) {
          BOOST_LOG_TRIVIAL(error) << "Could not allocate raw picture buffer\n";
        }
        this->sws_ctx = sws_getContext(
                this->width,
                this->height,
                AV_PIX_FMT_RGBA,
                this->codecWidth,
                this->codecHeight,
                AV_PIX_FMT_RGBA,
                SWS_FAST_BILINEAR,
                NULL,
                NULL,
                NULL
        );
    }

    this->encoder = NvPipe_CreateEncoder(NVPIPE_RGBA32, NVPIPE_H264, NVPIPE_LOSSY,this->bit_rate, this->fps);
    BOOST_LOG_TRIVIAL(info) << "Created NVENC encoder";



}

SRD_Buffer_Frame *NVENC_Encoder::encode(Image *image) {

    std::vector<uint8_t> compressed(this->codecWidth * this->codecHeight * 4);
    if(this->isScale) {
        unsigned char* srcData[1] = {(unsigned char * ) image->data}; // convert to array of pointer for plane
        const int inLinesize[1] = { 4 * this->width }; // bpp
        const int outLineSize[1] = {4 * this->codecWidth };
        unsigned char* dstData[1] = { (unsigned char * ) image->data };
        sws_scale(sws_ctx, (uint8_t const * const *) srcData, inLinesize, 0, this->height, dstData, outLineSize);

    }
    bool requestIFrame = false;
    if(this->interval >= 120) {
        requestIFrame = true;
        this->interval = 0;
    }


    uint64_t compressedSize = NvPipe_Encode(this->encoder, image->data, this->codecWidth * 4, compressed.data(), compressed.size(),this->codecWidth, this->codecHeight, requestIFrame);

    this->interval++;

    SRD_Buffer_Frame *encodedFrame = new SRD_Buffer_Frame();
    encodedFrame->data = (unsigned char *) malloc(compressedSize);
    std::memcpy(encodedFrame->data, compressed.data(), compressedSize);
    encodedFrame->size = compressedSize;
    encodedFrame->type = VIDEO_FRAME;

    return encodedFrame;
}

#endif