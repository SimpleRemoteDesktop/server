//
// Created by user on 16/04/18.
//

#include <cstdio>
#include "OpusEncoderManager.h"
#include "../encoder_ffmpeg.h"

#define FRAME_SIZE 960
#define MAX_PACKET_SIZE (3*1276)


OpusEncoderManager::OpusEncoderManager(int sampleRate, int channels, int bitrate) {


    int err;
    encoder = opus_encoder_create(sampleRate, channels, OPUS_APPLICATION_AUDIO, &err);
    if (err<0)
    {
        fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
        // FIXME exit
    }
    err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));
    if (err<0)
    {
        fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
        //FIXME exit
    }


}
void OpusEncoderManager::encode(unsigned char *in, Frame *frame, size_t size) {
    unsigned char* output;
    int nbBytes = opus_encode(encoder, (opus_int16*) in, size, output, MAX_PACKET_SIZE);
    if (nbBytes<0)
    {
        fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
        //FIXME exit
    }
    frame->data = output;
    frame->size = nbBytes;
    frame->type = AUDIO_FRAME;
}