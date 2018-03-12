#include "main.hpp"
#include "network.hpp"
#include "encoder_ffmpeg.h"
#include "xdisplay.h"

void video_thread_fn(float duration)
{
	while(true)
	{
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();	
		image = ( Image*  )malloc(sizeof(Image));
		image->data = (char*) malloc(sizeof(char)*width*height*4);	
		display_image(image, false);
		unsigned char * dstData = NULL;
		unsigned long frameSize = 0;
		encoder_encodeFrame( srcData, &dstData, &frameSize );
		//TODO send data to network buffer;
		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;

		boost::this_thread::sleep(boost::posix_time::milliseconds(duration - diff.total_milliseconds()));
	}
}

void start_video(int codecWidth, int codecHeight, int bandwidth, int fps, int sdl)
{
	const char* displayname = ":0";
	int desktopWidth = NULL;
	int desktopHeight = NULL;
	int desktopDepth = NULL;
	float duration = (float) 1 / fps;

	display_init(displayname, &desktopWidth, &desktopHeight, &desktopDepth);
	encoder_init(&desktopWidth, &desktopHeight, &codecWidth, &codecHeight, &bandwidth, &fps, 1);
	// start vide thread
	boost::thread videoThread(boost::bind(video_thread_fn, duration));
}

void stop_video()
{
	//TODO NOT IMPLEMENTED
}

void handle_incoming_message(Message* m)
{

	switch (message->type) {
		case 1:
			display_keypress_with_keysym(message->keycode, true);
			break;
		case 2:
			display_keypress_with_keysym(message->keycode, false);
			break;
		case 3:
			display_mouse_move(message->x, message->y);
			break;
		case 4:
			display_mouse_button(message->button, true);
			break;
		case 5:
			display_mouse_button(message->button, false);
			break;
		case 6:
			start_video(message->codecWidth, message->codecHeight, message->bandwidth, message->fps, message->sdl);
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
	cout << " Simple Remote desktop server version 0.2" << endl;
	// start network service
	SRD_server_init_listen();	
}
