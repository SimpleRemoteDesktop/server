//
// Created by user on 16/04/18.
//

#include "SoundManager.h"
#include "../encoder_ffmpeg.h"


void sound_capture_thread_fn(SoundManager* soundManager) { //TODO lambda
    soundManager->capture();
}

SoundManager::SoundManager(Fifo <Frame> outputQueue) {
    this->rawFifo = new Fifo<AudioPcm>;
    this->outputQueue = outputQueue;
    this->sampleRate = 48000;
    this->channels = 2;
    this->bitrate = 128000;
    this->pulse = new PulseAudioCapture(this->sampleRate, this->channels);
    this->encoder = new OpusEncoderManager(this->sampleRate, this->channels, this->bitrate);
}

void SoundManager::start() {

    fprintf(stdout, "sending start sound encoder:\n");
    boost::thread socketSendThread(boost::bind(sound_capture_thread_fn, this));


}

void SoundManager::capture() {
//FIXME : pulseaudio return buffer
    Frame* audioFrame = (Frame*) malloc(sizeof(Frame));
    size_t frame_sample_per_channel = this->sampleRate * 100 /* frame duration 2.5ms */ /10000LL;
    size_t frame_size = this->channels * frame_sample_per_channel * sizeof(short);
    fprintf(stdout, "frame_sample_per channel %d, frame size : %d \n", frame_sample_per_channel, frame_size);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    this->isRunning = true;
    unsigned char* buffer = (unsigned char *) malloc(frame_size);
    while(this->isRunning) {
        pulse->getBuffer(buffer, frame_size);
        encoder->encode(buffer, audioFrame, frame_sample_per_channel);
        fprintf(stdout, "sending audio frame size : %d\n", audioFrame->size);
        //this->outputQueue.push(frame);

    }
    free(buffer);
    fprintf(stdout, "sound loop exited");
}


