#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


#include "main.hpp"
#include "network.hpp"
#include "encoder_ffmpeg.h"
#include "xdisplay.h"
#include "keysym_converter.h"

Configuration* config;
Fifo<Frame> *queueToNetwork;
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
		SRD_X11_display_image(image, false);

		Frame* frame = new Frame();
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
	const char* displayname = ":0";
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

void handle_incoming_message(Message* message)
{

	switch (message->type) {
		case 1:
			SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), True);
			break;
		case 2:
			SRD_X11_display_keypress_with_keysym(get_keysym(message->keycode), False);
			break;
		case 3:
			SRD_X11_display_mouse_move(message->x,message->y); 
			break;
		case 4:
			SRD_X11_display_mouse_button(message->button, True);
			break;
		case 5:
			SRD_X11_display_mouse_button(message->button, False);
			break;
		case 6:
			BOOST_LOG_TRIVIAL(info) << "receive start request";
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

	boost::log::core::get()->set_filter (
		 boost::log::trivial::severity >= boost::log::trivial::info	
			);


	config = new Configuration();
	queueToNetwork = new Fifo<Frame>();
	queueFromNetwork = new Fifo<Message>();
	// init keysym mapper
	keysym_init();	
	BOOST_LOG_TRIVIAL(info) << " Simple Remote desktop server version 0.2";
	// start network service
	SRD_server_init_listen();

}
