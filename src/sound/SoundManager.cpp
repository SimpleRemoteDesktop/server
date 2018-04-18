//
// Created by user on 16/04/18.
//

#include "SoundManager.h"
#include "../encoder_ffmpeg.h"


void sound_capture_thread_fn(SoundManager* soundManager) { //TODO lambda
    soundManager->capture();
}

SoundManager::SoundManager(Fifo <Frame> outputQueue) {
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
    short buffer[64];
    Frame* frame = (Frame*) malloc(sizeof(Frame));
    this->isRunning = true;
    fprintf(stdout, "sound is running %d \n", this->isRunning);
    while(this->isRunning) { //TODO buffering 1ms of sound
        fprintf(stdout, "getting pulseaudio buffer\n");
        pulse->getBuffer(buffer);
        fprintf(stdout, "encoding with lib opus\n");
        encoder->encode(buffer, frame);
        fprintf(stdout, "sending frame size : %d\n", frame->size);

            this->outputQueue.push(frame);
    }
    fprintf(stdout, "sound loop exited");
}


