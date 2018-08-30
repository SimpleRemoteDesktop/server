//
// Created by sylvain on 29/08/18.
//

#include <X11/Xlib.h>
#include <cstring>
#include <cstdlib>
#include "X11Grab.h"

X11Grab::X11Grab() {

		this->displayName = std::getenv("DISPLAY");
		if (!this->displayName) {
			this->displayName = ":0";
		}

	printf("display  : %s\n ", this->displayName);
	// init
	//		XSetErrorHandler(handler);
	XInitThreads();
	int ignore = 0;
	memset(&this->xshminfo,0 ,sizeof(this->xshminfo));

	// open display
	if((this->display = XOpenDisplay(this->displayName)) == NULL) {
		printf("cannot open display \"%s\"\n", this->displayName ? this->displayName : "DEFAULT");
	}

	// check MIT extension
	if(XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore) ) {
		int major, minor;
		Bool pixmaps;
		if(XShmQueryVersion(display, &major, &minor, &pixmaps) == True) {
			printf("XShm extention version %d.%d %s shared pixmaps\n",
				   major, minor, (pixmaps==True) ? "with" : "without");
		} else {
			printf("XShm extension not supported.\n");
		}
	}
	// get default screen
	screenNumber = XDefaultScreen(display);
	if((screen = XScreenOfDisplay(display, screenNumber)) == NULL) {
		printf("cannot obtain screen #%d\n", screenNumber);
	}
	// get screen hight, width, depth
	this->width = XDisplayWidth(display, screenNumber); //TODO refactoring
	this->height = XDisplayHeight(display, screenNumber); //TODO refactoring
	int desktopDepth = XDisplayPlanes(display, screenNumber);
	printf("X-Window-init: dimension: %dx%dx%d @ %d/%d\n",
		   this->width, this->height, desktopDepth, screenNumber, XScreenCount(display));
	//create image context
	if((this->ximage = XShmCreateImage(display,
									   XDefaultVisual(display, screenNumber),
									   desktopDepth, ZPixmap, NULL, &this->xshminfo,
									   this->width, this->height)) == NULL) {
		printf("XShmCreateImage failed.\n");
	}


	//get shm info
	if((this->xshminfo.shmid = shmget(IPC_PRIVATE,
									  ximage->bytes_per_line*ximage->height,
									  IPC_CREAT | 0777)) < 0) {
		printf("shmget error");
	}

	//
	this->xshminfo.shmaddr = ximage->data = (char*) shmat(this->xshminfo.shmid, 0, 0);
	this->xshminfo.readOnly = False;
	if(XShmAttach(display, &this->xshminfo) == 0) {
		printf("XShmAttach failed.\n");
	}
	//
	rootWindow = XRootWindow(display, screenNumber);
}

Image* X11Grab::read() {

	printf("screen capture X11 windows");
	XLockDisplay(display);
	if(XShmGetImage(display, rootWindow, ximage, 0, 0, XAllPlanes()) == 0) {
		// image->data  BGRA
		printf("FATAL: XShmGetImage failed.\n");
		exit(-1);
	}
	XUnlockDisplay(display);

	Image* image = new Image();
	image->data = (char*) malloc(ximage->width * ximage->height * 4);
	image->width = ximage->width;
	image->height = ximage->height;
	memcpy(image->data, ximage->data, ximage->width * ximage->height * 4);
	image->depth = ximage->depth;
	image->bits_per_pixel = ximage->bits_per_pixel;
	image->bytes_per_line = ximage->bytes_per_line;

	return image;
}
