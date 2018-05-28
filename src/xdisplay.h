

#ifndef XDISPLAY_H
#define XDISPLAY_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include<stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
	static int MOUSE_LEFT = 1;
	static int MOUSE_MIDDLE = 2;
	static int MOUSE_RIGHT = 3;


	typedef struct {
		int width;
		int height;
		char * data;
		int depth;
		int bytes_per_line;
		int bits_per_pixel;

	} Image;

	int SRD_X11_display_init(const char * displayname, Configuration* config);
	void SRD_X11_display_image( Image *image, int withPointer );
	void SRD_X11_display_keypress_with_keycode( int keycode, int isDown );
	void SRD_X11_display_keypress_with_keysym( int keysym, int isDown );
	void SRD_X11_display_mouse_move( float x, float y );
	void SRD_X11_display_mouse_button(int button, int isDown );
	void SRD_X11_paint_mouse_pointer(Image *image);
	void SRD_X11_display_close();
#ifdef __cplusplus
}
#endif
#endif
