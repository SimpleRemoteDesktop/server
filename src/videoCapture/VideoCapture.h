//
// Created by sylvain on 23/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_VIDEOCAPTURE_H
#define SIMPLEREMOTEDESKTOP_SERVER_VIDEOCAPTURE_H


#include "./videoGrab/FrameBufferGrab.h"
#include "./videoEncoder/SoftwareEncoder.h"
#include "./videoEncoder/NVENC_Encoder.h"
#include "../fifo.hpp"
#include "videoGrab/X11Grab.h"
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

class VideoCapture {
public:
    VideoCapture(int codecWidth, int codecHeight, int bit_rate, int fps, Fifo<SRD_Buffer_Frame> *queueToNetwork, bool withNvEnc);
    ~VideoCapture();
    void start();
    void stop();

    void capture();

private:
    X11Grab *grab;
    //SoftwareEncoder *encoder;
    AbstractEncoder* encoder;
    int codecWidth;
    int codecHeight;
    int bit_rate;
    int fps;
    float duration;
    bool isRunningThread = false;
    Fifo<SRD_Buffer_Frame> *outputQueue;
    bool withNvEnc;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_VIDEOCAPTURE_H
