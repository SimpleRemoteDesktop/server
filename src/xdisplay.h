
#ifndef XDISPLAY_H
#define XDISPLAY_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include<stdint.h>
#include <stdbool.h>
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


#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
	static int MOUSE_LEFT = 1;
	static int MOUSE_MIDDLE = 2;
	static int MOUSE_RIGHT = 3;


	struct Image {
		int width;
		int height;
		char * data;
		int depth;
		int bytes_per_line;
		int bits_per_pixel;

	};

	int SRD_X11_display_init(const char * displayname, int * desktopWidth, int * desktopHeight, int *desktopDepth);
	void SRD_X11_display_image( Image *image, bool withPointer );
	void SRD_X11_display_keypress_with_keycode( int keycode, bool isDown );
	void SRD_X11_display_keypress_with_keysym( int keysym, bool isDown );
	void SRD_X11_display_mouse_move( int x, int y );
	void SRD_X11_display_mouse_button(int button, bool isDown );
	void SRD_X11_paint_mouse_pointer(Image *image);
	void SRD_X11_display_close();
#ifdef __cplusplus
}
#endif
#endif
