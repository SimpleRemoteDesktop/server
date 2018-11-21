//
// Created by user on 16/04/18.
//

#include "SoundManager.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

void sound_capture_thread_fn(SoundManager* soundManager) { //TODO lambda
    soundManager->capture();
}

SoundManager::SoundManager(Fifo<SRD_Buffer_Frame> * outputqueue) {
    BOOST_LOG_TRIVIAL(debug) << "creating pulseaudio context";
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
        BOOST_LOG_TRIVIAL(error) << "audio source: pulseaudio initialization failed - "<< error<< " : " << pa_strerror(error);
    }
    pa_simple_get_latency(pa_ctx, &error);
    if(error)  {
        BOOST_LOG_TRIVIAL(error) << " unable to retreive pulseaudio latency "; // << error << " " << pa_strerror(error);
    }
    BOOST_LOG_TRIVIAL(debug) << " plusaudio latency " << delay;


    BOOST_LOG_TRIVIAL(debug) << "init pulseaudio ended";



    int err;
    encoder = opus_encoder_create(48000, 2, OPUS_APPLICATION_AUDIO, &err);
    if (err<0)
    {
        BOOST_LOG_TRIVIAL(error) << "failed to create an encoder: " << opus_strerror(err);
        exit(1);
    }
    err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(128000));
    if (err<0)
    {
        BOOST_LOG_TRIVIAL(error) << "failed to set bitrate: " << opus_strerror(err);
        exit(1);
    }
    BOOST_LOG_TRIVIAL(debug) << "init finished";
}

boost::thread SoundManager::start() {

    BOOST_LOG_TRIVIAL(info) << "sending start sound encoder";
    boost::thread socketSendThread(boost::bind(sound_capture_thread_fn, this));
    return socketSendThread;


}

void SoundManager::capture() {
    this->isRunning = true;
    while(this->isRunning) {


        SRD_Buffer_Frame* srd_buffer_frame = new SRD_Buffer_Frame();
        unsigned char * buffer = (unsigned char*) malloc(3840);
        memset(buffer, 0, 3840);
        int err = pa_simple_read(pa_ctx, buffer, 3840, &error);
        if( err < 0) { //FIXME size
            BOOST_LOG_TRIVIAL(error) << "pulseaudio read failed " << error; //TODO throw error
        }
        unsigned char* output;
        output = (unsigned char*) malloc(3*1276);
        int nbBytes;
        if(this->isRunning) {
            nbBytes = opus_encode(encoder, (opus_int16 *) buffer, 960, output, 3840 * 8);
            if (nbBytes < 0) {
                BOOST_LOG_TRIVIAL(error) << "encode failed: " << opus_strerror(nbBytes);
                //FIXME exit
            }
        }
        if(this->isRunning) {
            //fprintf(stdout, "encoder output nbBytes %d \n",nbBytes); //TODO
            srd_buffer_frame->size = nbBytes;
            srd_buffer_frame->data = output;
            srd_buffer_frame->type = SRD_AUDIO_FRAME;
            free(buffer);

            outputqueue->push(srd_buffer_frame);
        }

    }

    pa_simple_free(this->pa_ctx);
}

void SoundManager::stop() {
    BOOST_LOG_TRIVIAL(info) << "stopping sound manager loop";
    this->isRunning = false;
}

