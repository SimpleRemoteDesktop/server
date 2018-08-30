//
// Created by sylvain on 29/08/18.
//

#include <vector>
#include <cstring>
#include "NVENC_Encoder.h"

NVENC_Encoder::NVENC_Encoder(int width, int height, int bit_rate, int fps) {
    this->width = width;
    this->height = height;
    this->bit_rate = bit_rate;
    this->fps = fps;
    this->encoder = NvPipe_CreateEncoder(NVPIPE_BGRA32, NVPIPE_H264, NVPIPE_LOSSY,this->bit_rate, this->fps);

}

SRD_Buffer_Frame *NVENC_Encoder::encode(Image *image) {

    std::vector<uint8_t> compressed(this->width * this->height * 4);
    uint64_t compressedSize = NvPipe_Encode(this->encoder, image->data, width * 4, compressed.data(), compressed.size(), this->width, this->height, false);

    SRD_Buffer_Frame *encodedFrame = new SRD_Buffer_Frame();
    encodedFrame->data = (unsigned char *) malloc(compressedSize);
    std::memcpy(encodedFrame->data, compressed.data(), compressedSize);
    encodedFrame->size = compressedSize;
    encodedFrame->type = VIDEO_FRAME;

    return encodedFrame;
}
