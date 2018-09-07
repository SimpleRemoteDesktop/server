//
// Created by sylvain on 05/09/18.
//

#include "AppManager.h"
#include "network.hpp"


AppManager::AppManager(bool withNvEnc) {
	this->withNvEnc = withNvEnc;
	this->queueToNetwork = new Fifo<SRD_Buffer_Frame>();
	this->queueFromNetwork = new Fifo<Message>();

	this->initInput();
	SRD_server_init_listen();
}

void AppManager::initInput() {
	// init keysym mapper
	//keysym_init();
	this->x11input = new X11FakeInput(":0");
	this->kb = new SRD_Keyboard();
	this->mouse = new SRD_Mouse();
}

void AppManager::start(bool withSound) {
	//TODO check
	this->videoCapture = new VideoCapture(this->codecWidth, this->codecHeight, this->bandwidth, this->fps,
			this->queueToNetwork, this->withNvEnc);
	this->videoCapture->start();

	if(withSound) {
		this->soundManager = new SoundManager(queueToNetwork);
		this->soundManager->start();
	}

}
void AppManager::messageHandler(Message* message) {
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
			x11input->mouseBtton(message->button, True);
			break;
		case TYPE_MOUSE_UP:
			x11input->mouseBtton(message->button, False);
			break;
		case TYPE_MOUSE_MOTION:
			x11input->mouseMove(message->x, message->y);
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

			this->start(true);
			break;
		case TYPE_ENCODER_STOP:
			this->stop();
			break;
		default:
			break;

	}
}

void AppManager::stop() {
    //TODO

}
