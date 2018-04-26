//
// Created by user on 16/04/18.
//

#include "SoundManager.h"
#include "../encoder_ffmpeg.h"


void sound_capture_thread_fn(SoundManager* soundManager) { //TODO lambda
    soundManager->capture();
}

SoundManager::SoundManager(Fifo<SRD_Buffer_Frame> * outputqueue) {
    fprintf(stdout, "creating pulseaudio context\n");
    this->outputqueue = outputqueue;
    pa_usec_t delay = 0;

    // TODO bitrate 128000

    pa_sample_spec pa_spec;
    memset(&pa_spec, 0, sizeof(pa_spec));
    pa_spec.channels = 2;
    pa_spec.rate = 48000;
    pa_spec.format = PA_SAMPLE_S16LE;

    pa_ctx = pa_simple_new(NULL, "Simple Remote desktop audio source", PA_STREAM_RECORD, NULL, "srd record stream", &pa_spec, NULL, NULL, &error);
    if(pa_ctx == NULL) {
        fprintf(stderr, "audio source: pulseaudio initialization failed - %d:%s.\n", error, pa_strerror(error));
    }
    pa_simple_get_latency(pa_ctx, &error);
    if(error)  {
        fprintf(stderr, " unable to retreive pulseaudio latency \n %d \n %s \n", error,pa_strerror(error));
    }
    fprintf(stdout, " plusaudio latency %d \n", delay);


    fprintf(stdout, "init pulseaudio ended \n");



    int err;
    encoder = opus_encoder_create(48000, 2, OPUS_APPLICATION_AUDIO, &err);
    if (err<0)
    {
        fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
        exit(1);
    }
    fprintf(stdout, "coucou\n");
    err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(128000));
    if (err<0)
    {
        fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
        exit(1);
    }
    fprintf(stdout, "init finished \n");
}

void SoundManager::start() {

    fprintf(stdout, "sending start sound encoder:\n");
    boost::thread socketSendThread(boost::bind(sound_capture_thread_fn, this));


}

void SoundManager::capture() {
    while(true) {


        SRD_Buffer_Frame* srd_buffer_frame = new SRD_Buffer_Frame();
        unsigned char * buffer = (unsigned char*) malloc(1920);
        int err = pa_simple_read(pa_ctx, buffer, 1920, &error);
        if( err < 0) { //FIXME size
            fprintf(stderr, "pulseaudio read failed %d \n", error); //TODO throw error
        }
        unsigned char* output;
        int nbBytes = opus_encode(encoder, (opus_int16*) buffer, 480, output, 1920);
        if (nbBytes<0)
        {
            fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
            //FIXME exit
        }
        //fprintf(stdout, "encoder output nbBytes %d \n",nbBytes); //TODO
        srd_buffer_frame->size = nbBytes;
        srd_buffer_frame->data = output;
        srd_buffer_frame->type = SRD_AUDIO_FRAME;
        free(buffer);

        outputqueue->push(srd_buffer_frame);
    }
}

