//
// Created by sylvain on 05/09/18.
//

#include "AppManager.h"

AppManager::AppManager(bool withNvEnc, bool withSound) {
    this->withNvEnc = withNvEnc;
    this->withSound = withSound;
    this->queueToNetwork = new Fifo<SRD_Buffer_Frame>();
    this->queueFromNetwork = new Fifo<Message>();
    this->port = 8001;

    this->network = new Network(this->port, this->queueFromNetwork, this->queueToNetwork);
}

void AppManager::initInput() {
    // init keysym mapper
    //keysym_init();
    this->x11input = new X11FakeInput(":0");
    this->kb = new SRD_Keyboard();
    this->mouse = new SRD_Mouse();
    this->touchscreen = new SRD_Touchscreen(this->x11input->getWidth(), this->x11input->getHeight());
}

void AppManager::startStream() {
    //TODO check
    this->videoCapture = new VideoCapture(this->codecWidth, this->codecHeight, this->bandwidth, this->fps,
                                          this->queueToNetwork, this->withNvEnc);
    if (this->withSound) {
        this->soundManager = new SoundManager(queueToNetwork);
    }

    this->videoThread = this->videoCapture->start();

    if (this->withSound) {
        this->soundThread = this->soundManager->start();
    }

}

void AppManager::messageLoop() {
    BOOST_LOG_TRIVIAL(info) << "entering Message loop";
    bool isWaitingMessage = true;
    while (isWaitingMessage) {
        Message *message = this->queueFromNetwork->get();
        if (message) {
            switch (message->type) {
                case TYPE_KEY_DOWN:

                    fprintf(stdout, "keycode %d, scancode %d \n", message->keycode, message->scancode);
                    //SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), 1);
                    kb->press(message->scancode, 1);
                    break;
                case TYPE_KEY_UP:
                    //SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), 0);
                    kb->press(message->scancode, 0);
                    break;
                case TYPE_MOUSE_DOWN:
                    this->touchscreen->mouseButton(message->button, true);
                    //x11input->mouseBtton(message->button, True);
                    break;
                case TYPE_MOUSE_UP:
                    this->touchscreen->mouseButton(message->button, false);
                    //x11input->mouseBtton(message->button, False);
                    break;
                case TYPE_MOUSE_MOTION:
                    this->touchscreen->mouseMove(message->x, message->y);
                    //x11input->mouseMove(message->x, message->y);
                    break;
                case TYPE_MOUSE_RELATIVE_MOTION:
                    fprintf(stdout, "relative mouse x: %d, y: %d \n", message->x, message->y);
                    this->mouse->mouseMove(message->x, message->y);
                    break;
                case TYPE_ENCODER_START:
                    BOOST_LOG_TRIVIAL(info) << "receive start request";
                    BOOST_LOG_TRIVIAL(info) << " codec width " << message->codec_width;
                    BOOST_LOG_TRIVIAL(info) << " codec height " << message->codec_height;
                    BOOST_LOG_TRIVIAL(info) << " codec bandwidth " << message->bandwidth;
                    BOOST_LOG_TRIVIAL(info) << " codec fps " << message->fps;

                    this->codecWidth = message->codec_width;
                    this->codecHeight = message->codec_height;
                    this->bandwidth = message->bandwidth;
                    this->fps = message->fps;

                    this->startStream();
                    break;
                case TYPE_ENCODER_STOP:
                    BOOST_LOG_TRIVIAL(info) << "Receive stop stream";
                    isWaitingMessage = false;
                    this->stopStream();
                    break;
                default:
                    break;

            }
        } else {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Exited message loop";
}

void AppManager::stop() {
    //TODO

}

void AppManager::start() {
    this->initInput();
    this->appLoop();

}

void AppManager::stopStream() {
    this->videoCapture->stop();
    this->videoThread.join();
    this->soundManager->stop();
    this->soundThread.join();
    BOOST_LOG_TRIVIAL(debug) << "cleaning SRD Frame Buffer queue";
    bool clean = true;
    while (clean) {
        SRD_Buffer_Frame *frame = this->queueToNetwork->get();
        if (!frame) {
            clean = false;
        } else {
            free(frame);
        }
    }
}

void AppManager::appLoop() {
    this->network->listen();
    this->messageLoop();
    BOOST_LOG_TRIVIAL(info) << "appLoop, retstarting network listen";
    this->appLoop();

}
