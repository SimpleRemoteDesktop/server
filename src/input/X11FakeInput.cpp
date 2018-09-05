//
// Created by sylvain on 29/08/18.
//

#include <X11/extensions/XTest.h>
#include <cstdio>
#include <cstdlib>
#include "X11FakeInput.h"
#include "../config.h"

X11FakeInput::X11FakeInput(const char* displayName) {
		XInitThreads();
		int ignore = 0;
		this->displayName = std::getenv("DISPLAY");
		if (!this->displayName) {
			this->displayName = ":0";
		}


		// open display
		if((display = XOpenDisplay(this->displayName)) == NULL) {
			printf("cannot open display \"%s\"\n", this->displayName ? this->displayName : "DEFAULT");
		}

		// get default screen
		this->screenNumber = XDefaultScreen(display);
		if((this->screen = XScreenOfDisplay(display, screenNumber)) == NULL) {
			printf("cannot obtain screen #%d\n", screenNumber);
		}
		// get screen hight, width, depth
		this->width = XDisplayWidth(display, screenNumber); //TODO refactoring
		this->height = XDisplayHeight(display, screenNumber); //TODO refactoring
		int desktopDepth = XDisplayPlanes(display, screenNumber);
		printf("X-Window-init: dimension: %dx%dx%d @ %d/%d\n",
				this->width, this->height, desktopDepth, screenNumber, XScreenCount(display));

}

void X11FakeInput::keypress_with_keycode( int keycode, int isDown )
{
    XLockDisplay(this->display);
    XTestGrabControl(display, True);
    XTestFakeKeyEvent(display,keycode, isDown ? True : False, CurrentTime);
    XFlush(display);
    XTestGrabControl(display, False);
    XUnlockDisplay(display);
}

 void X11FakeInput::keypress_with_keysym( int keysym, int isDown )
{

    XLockDisplay(display);
    XTestGrabControl(display, True);
    XTestFakeKeyEvent(display,XKeysymToKeycode(display, keysym), isDown ? True : False, CurrentTime) ;//FIXME isdown
    XFlush(display);
    XTestGrabControl(display, False);
    XUnlockDisplay(display);
}

void X11FakeInput::mouseMove( float x, float y ) // TODO must be int => convertor
{
    int a = (int) this->width * x;
    int b = (int) this->height *y;
    //printf("mouse x: %d, y: %d", a, b);
    XLockDisplay(display);
    XTestGrabControl(display, True);
    XTestFakeMotionEvent(display, screenNumber, a, b, CurrentTime);
    XFlush(display);
    XTestGrabControl(display, False);
    XUnlockDisplay(display);
}

void X11FakeInput::mouseBtton(int button, int isDown )
{
    printf("button %d, is down %d\n", button, isDown);
    XLockDisplay(display);
    XTestGrabControl(display, True);
    XTestFakeButtonEvent(display, button, isDown ? True : False, CurrentTime);
    XFlush(display);
    XTestGrabControl(display, False);
    XUnlockDisplay(display);
}


