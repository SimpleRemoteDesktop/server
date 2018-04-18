//
// Created by user on 16/04/18.
//

#include <cstring>
#include "PulseAudioCapture.h"
using namespace std;


PulseAudioCapture::PulseAudioCapture(int sampleRate, int channels) {
    fprintf(stdout, "creating pulseaudio context");
    //dev = "auto_null.monitor";
    dev = "xrdp-sink.monitor";
    char pa_devname[64];
    pa_usec_t delay = 0;

    // TODO bitrate 128000

    pa_sample_spec pa_spec;
    memset(&pa_spec, 0, sizeof(pa_spec));
    pa_spec.channels = channels; //2
    pa_spec.rate = sampleRate; //48000
    pa_spec.format = PA_SAMPLE_S16LE;

    pa_ctx = pa_simple_new(NULL, "Simple Remote desktop audio source", PA_STREAM_RECORD, NULL, "srd record stream", &pa_spec, NULL, NULL, &error);
    if(pa_ctx == NULL) {
        fprintf(stderr, "audio source: pulseaudio initialization failed - %d:%s.\n", error, pa_strerror(error));
    }
    pa_simple_get_latency(pa_ctx, &error);
    if(error)  {
        fprintf(stderr, " unable to retreive pulseaudio latency \n %d \n %s \n", error,pa_strerror(error));
    } else {
        fprintf(stdout, " plusaudio latency %dus \n", delay);
    }

}

void PulseAudioCapture::getBuffer(unsigned char* buffer) {

    if(pa_simple_read(pa_ctx, buffer, 1920, &error) < 0) { //FIXME size
        fprintf(stderr, "pulseaudio read failed %d \n", error); //TODO throw error
    }
}

PulseAudioCapture::~PulseAudioCapture() {
    fprintf(stdout, "deleting pulseaudio context");
    if(pa_ctx != NULL)
        pa_simple_free(pa_ctx);
    pa_ctx = NULL;
}
