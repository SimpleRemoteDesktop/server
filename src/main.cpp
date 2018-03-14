#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 

#include "main.hpp"
#include "network.hpp"
#include "encoder_ffmpeg.h"
#include "xdisplay.h"


Configuration* config;
Fifo<Frame> *queueToNetwork;
Fifo<Message> *queueFromNetwork;


void video_thread_fn(float duration)
{
	while(true)
	{
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();	
		Image* image = ( Image*  )malloc(sizeof(Image));
		image->data = (char*) malloc(sizeof(char)*config->width*config->height*4);	
		SRD_X11_display_image(image, false);

		Frame* frame = new Frame();
		frame->data = NULL;
		frame->size = 0;

		encoder_encodeFrame(image, frame);
		queueToNetwork->push(frame);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;

		boost::this_thread::sleep(boost::posix_time::milliseconds(duration - diff.total_milliseconds()));
	}
}

void start_video(int codecWidth, int codecHeight, int bandwidth, int fps, int sdl)
{
	const char* displayname = ":0";
	float duration = (float) 1 / fps;

	SRD_X11_display_init(displayname, config);
	encoder_init(config, &codecWidth, &codecHeight, &bandwidth, &fps, 1);
	// start vide thread
	boost::thread videoThread(boost::bind(video_thread_fn, duration));
}

void stop_video()
{
	//TODO NOT IMPLEMENTED
}

void handle_incoming_message(Message* message)
{

	switch (message->type) {
		case 1:
			SRD_X11_display_keypress_with_keysym(message->keycode, true);
			break;
		case 2:
			SRD_X11_display_keypress_with_keysym(message->keycode, false);
			break;
		case 3:
			SRD_X11_display_mouse_move(message->x, message->y);
			break;
		case 4:
			SRD_X11_display_mouse_button(message->button, true);
			break;
		case 5:
			SRD_X11_display_mouse_button(message->button, false);
			break;
		case 6:
			std::cout << "receive start request" << std::endl;
			start_video(message->codec_width, message->codec_height, message->bandwidth, message->fps, message->sdl);
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




	config = new Configuration();
	queueToNetwork = new Fifo<Frame>();
	queueFromNetwork = new Fifo<Message>();
	std::cout << " Simple Remote desktop server version 0.2" << std::endl;
	// start network service
	SRD_server_init_listen();

}
