//
// Created by user on 16/04/18.
//

#include <cstring>
#include <boost/thread.hpp>
#include "PulseAudioCapture.h"

using namespace std;


PulseAudioCapture::PulseAudioCapture(int sampleRate, int channels) {
    fprintf(stdout, "creating pulseaudio context");
    pa_usec_t delay = 0;

    // TODO bitrate 128000

    memset(&this->pa_spec, 0, sizeof(this->pa_spec));
    this->pa_spec.channels = channels; //2
    this->pa_spec.rate = sampleRate; //48000
    this->pa_spec.format = PA_SAMPLE_S16LE;

    this->pa_ctx = pa_simple_new(NULL, "Simple Remote desktop audio source", PA_STREAM_RECORD, NULL, "srd record stream", &this->pa_spec, NULL, NULL, &error);
    if(this->pa_ctx == NULL) {
        fprintf(stderr, "audio source: pulseaudio initialization failed - %d:%s.\n", error, pa_strerror(error));
    }
    pa_simple_get_latency(this->pa_ctx, &error);
    if(error)  {
        fprintf(stderr, " unable to retreive pulseaudio latency \n %d \n %s \n", error,pa_strerror(error));
    } else {
        fprintf(stdout, " plusaudio latency %d \n", delay);
    }

}

void PulseAudioCapture::getBuffer(unsigned char* buffer, size_t size) {

    if(pa_simple_read(this->pa_ctx, buffer, size, &error) < 0) {
        fprintf(stderr, "pulseaudio read failed %d \n", error); //TODO throw error
    }
}


PulseAudioCapture::~PulseAudioCapture() {
    fprintf(stdout, "deleting pulseaudio context");
    if(this->pa_ctx != NULL)
        pa_simple_free(this->pa_ctx);
    this->pa_ctx = NULL;
}

/*void pulse_thread_fn(PulseAudioCapture * pulse) {
    unsigned char* buffer = (unsigned char *) malloc(480*2*sizeof(short));
//    if(pa_simple_read(pa))

}

void PulseAudioCapture::grab() {
    boost::thread socketSendThread(boost::bind(pulse_thread_fn, this)); // TODO lamnbda
}*/

