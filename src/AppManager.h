//
// Created by sylvain on 05/09/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_APPMANAGER_H
#define SIMPLEREMOTEDESKTOP_SERVER_APPMANAGER_H

#include "fifo.hpp"
#include "config.h"
#include "input/X11FakeInput.h"
#include "input/SRD_Keyboard.h"
#include "input/SRD_Mouse.h"
#include "videoCapture/VideoCapture.h"
#include "sound/SoundManager.h"
#include "network/Network.h"
#include "input/SRD_Touchscreen.h"

class AppManager {
public:
    AppManager(bool withNvEnc, bool withSound);
    ~AppManager();
    void start();
    void stop();


private:
    void messageLoop();
    void startStream();
    void stopStream();
    void initInput();
    void appLoop();
    bool withNvEnc;
    Fifo<SRD_Buffer_Frame> *queueToNetwork;
    Fifo<Message> *queueFromNetwork;
    X11FakeInput *x11input;
    SRD_Touchscreen * touchscreen;
    SRD_Keyboard *kb;
    SRD_Mouse *mouse;
    VideoCapture *videoCapture;
    SoundManager *soundManager;
    boost::thread videoThread;
    boost::thread soundThread;

    int codecWidth;
    int codecHeight;
    int bandwidth;
    int fps;
    int port;
    Network *network;
    bool withSound = true;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_APPMANAGER_H
