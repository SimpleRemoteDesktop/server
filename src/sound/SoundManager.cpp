//
// Created by user on 16/04/18.
//

#include "SoundManager.h"


void sound_capture_thread_fn(SoundManager* soundManager) { //TODO lambda
    soundManager->capture();
}

SoundManager::SoundManager(Fifo <Frame> outputQueue) {
    this->outputQueue = outputQueue;
    this->sampleRate = 48000;
    this->channels = 2;
    this->bitrate = 128000;
    this->pulse = new PulseAudioCapture(this->sampleRate, this->channels);
    this->encoder = new OpusEncoder(this->sampleRate, this->channels, this->bitrate);
}

void SoundManager::start() {
    boost::thread socketSendThread(boost::bind(sound_capture_thread_fn, this));
}

void SoundManager::capture() {

    unsigned char* buffer = NULL;
    Frame* frame = NULL;
    int duration = 1 / this->sampleRate;
    this->isRunning = true;
    while(this->isRunning) {
        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
        pulse->getBuffer(buffer);
        encoder->encode((short*) buffer, frame);
        outputQueue.push(frame);
        boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = t2 - t1;
        //BOOST_LOG_TRIVIAL(debug) << "encoder time : " << diff.total_milliseconds();

        boost::this_thread::sleep(boost::posix_time::milliseconds(duration - diff.total_milliseconds()));

    }
}


