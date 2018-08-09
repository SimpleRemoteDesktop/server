#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "network.hpp"
#include "encoder_ffmpeg.h"
#include "main.hpp"
#include "xdisplay.h"
#include "input/SD2_X11_keysym_converter.h"
#include "input/SRD_Keyboard.h"
#include "input/SRD_Mouse.h"
#include "config.h"

Configuration* config;
SRD_Keyboard *kb;
SRD_Mouse *mouse;
Fifo<SRD_Buffer_Frame> *queueToNetwork;
Fifo<Message> *queueFromNetwork;
bool video_thread_is_running = false;


void video_thread_fn(float duration)
{
	BOOST_LOG_TRIVIAL(info) << "Starting new video thread with loop duration : " << duration << " ms";
	while(video_thread_is_running)
	{
		boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time(); 
		Image* image = ( Image*  )malloc(sizeof(Image));
		image->data = (char*) malloc(sizeof(char)*config->width*config->height*4);	
		SRD_X11_display_image(image, true);

		SRD_Buffer_Frame* frame = new SRD_Buffer_Frame();
		frame->data = NULL;
		frame->size = 0;

		encoder_encodeFrame(image, frame);
		free(image->data);
		free(image);
		queueToNetwork->push(frame);

		boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time(); 
		boost::posix_time::time_duration diff = t2 - t1;
		BOOST_LOG_TRIVIAL(debug) << "encoder time : " << diff.total_milliseconds();

		boost::this_thread::sleep(boost::posix_time::milliseconds(duration - diff.total_milliseconds()));
	} 

}

void start_video(int codecWidth, int codecHeight, int bandwidth, int fps, int sdl)
{
	const char* displayname = std::getenv("DISPLAY");
	float duration = (float) 1000 / fps;
	BOOST_LOG_TRIVIAL(info) << "fps : " << fps << ", duration " << duration;
	SRD_X11_display_init(displayname, config);
	encoder_init(config, &codecWidth, &codecHeight, &bandwidth, &fps, 1);
	// start video thread
	video_thread_is_running = true;
	boost::thread videoThread(boost::bind(video_thread_fn, duration));
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

            BOOST_LOG_TRIVIAL(info) << "mouse move x :" << message->x << "y : "  << message->y;
		    mouse->mouseMove(message->x, message->y);

			//SRD_X11_display_mouse_move(message->x,message->y);
			break;
		case 4:
		    mouse->mouseButton(message->button, 1);
			//SRD_X11_display_mouse_button(message->button, True);
			break;
		case 5:
		    mouse->mouseButton(message->button, 0);
			//SRD_X11_display_mouse_button(message->button, False);
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
			boost::log::trivial::severity >= boost::log::trivial::info
			);


	config = new Configuration();
	queueToNetwork = new Fifo<SRD_Buffer_Frame>();
	queueFromNetwork = new Fifo<Message>();
	// init keysym mapper
	//keysym_init();
	kb = new SRD_Keyboard();
	mouse = new SRD_Mouse();
	BOOST_LOG_TRIVIAL(info) << " Simple Remote desktop server version 0.2";
	// start network service
	SRD_server_init_listen();

}
