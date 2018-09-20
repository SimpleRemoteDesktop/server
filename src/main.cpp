#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "network.hpp"
#include "main.hpp"
#include "input/SD2_X11_keysym_converter.h"
#include "input/SRD_Keyboard.h"
#include "input/SRD_Mouse.h"
#include "config.h"
#include "videoCapture/VideoCapture.h"
#include "input/X11FakeInput.h"
#include "input/SRD_Touchscreen.h";
Configuration* config;
SRD_Keyboard *kb;
SRD_Mouse *mouse;
SRD_Touchscreen* touchscreen;
Fifo<SRD_Buffer_Frame> *queueToNetwork;
Fifo<Message> *queueFromNetwork;
bool video_thread_is_running = false;
FrameBufferGrab* fbgrab;
X11FakeInput* x11input;
bool withNvEnc = false;



void start_video(int codecWidth, int codecHeight, int bandwidth, int fps, int sdl)
{
	VideoCapture *vc = new VideoCapture(codecWidth, codecHeight, bandwidth, fps, queueToNetwork, withNvEnc);
	vc->start();

    /*fbgrab = new FrameBufferGrab();
	const char* displayname = std::getenv("DISPLAY");
	float duration = (float) 1000 / fps;
	BOOST_LOG_TRIVIAL(info) << "fps : " << fps << ", duration " << duration;
	SRD_X11_display_init(displayname, config);
	encoder_init(config, &codecWidth, &codecHeight, &bandwidth, &fps, 1);
	// start video thread
	video_thread_is_running = true;
	boost::thread videoThread(boost::bind(video_thread_fn, duration));*/
}

void stop_video()
{
	BOOST_LOG_TRIVIAL(info) << " stoping video encoder" << std::endl;
	video_thread_is_running = false;
}


void start_sound()
{
	SoundManager *soundManager = new SoundManager(queueToNetwork);
	soundManager->start();
}

void handle_incoming_message(Message* message)
{
	switch (message->type) {
		case 1:

			fprintf(stdout, "keycode %d, scancode %d \n", message->keycode, message->scancode);
			//SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), 1);
			kb->press(message->scancode, 1);
			break;
		case 2:
			//SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), 0);
			fprintf(stdout, "keycode %d, scancode %d \n", message->keycode, message->scancode);
			kb->press(message->scancode, 0);
			break;
		case 3:
			//x11input->mouseMove(message->x, message->y);
			touchscreen->mouseMove(message->x, message->y);
			break;
		case 4:
			//x11input->mouseBtton(message->button, True);
			touchscreen->mouseButton(message->button, True);
			break;
		case 5:
			//x11input->mouseBtton(message->button, False);
			touchscreen->mouseButton(message->button, False);
			break;
		case 6:
			BOOST_LOG_TRIVIAL(info) << "receive start request";
			BOOST_LOG_TRIVIAL(info) << " codec width " << message->codec_width;
			BOOST_LOG_TRIVIAL(info) << " codec height " << message->codec_height;
			BOOST_LOG_TRIVIAL(info) << " codec bandwidth " << message->bandwidth;
			BOOST_LOG_TRIVIAL(info) << " codec fps " << message->fps;
			BOOST_LOG_TRIVIAL(info) << " use sdl" << message->sdl;

			start_video(message->codec_width, message->codec_height, message->bandwidth, message->fps, message->sdl);
			start_sound();
			break;
		case 7:
			stop_video();
			break;
		default:
			break;

	}
}


int main(int argc, const char* argv[])
{

	boost::log::core::get()->set_filter (
			boost::log::trivial::severity >= boost::log::trivial::debug
			);


	if(argc == 2) {
		std::string key = argv[1];
		if(key.compare("+nvenc") == 0) {
			withNvEnc = true;
		}
	}

	config = new Configuration();
	queueToNetwork = new Fifo<SRD_Buffer_Frame>();
	queueFromNetwork = new Fifo<Message>();
	// init keysym mapper
	//keysym_init();
	x11input = new X11FakeInput(":0");
	touchscreen = new SRD_Touchscreen(x11input->getWidth(), x11input->getHeight());
	kb = new SRD_Keyboard();
	//mouse = new SRD_Mouse();

	BOOST_LOG_TRIVIAL(info) << " Simple Remote desktop server version 0.2";
	// start network service
	SRD_server_init_listen();

}
