//
// Created by user on 16/04/18.
//

#include <cstdio>
#include "OpusEncoder.h"

#define FRAME_SIZE 960
#define MAX_PACKET_SIZE (3*1276)


OpusEncoder::OpusEncoder(int sampleRate, int channels, int bitrate) {


    int nbBytes;
    int err;
    opus_int16 in[FRAME_SIZE*channels];
    unsigned char cbits[MAX_PACKET_SIZE];
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
void OpusEncoder::encode(short *in, Frame * frame) {
    unsigned char* output;
    int nbBytes = opus_encode(encoder, in, FRAME_SIZE, output, MAX_PACKET_SIZE);
    if (nbBytes<0)
    {
        fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
        //FIXME exit
    }
    frame->data = output;
    frame->size = nbBytes;
}