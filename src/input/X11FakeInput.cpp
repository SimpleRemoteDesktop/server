//
// Created by sylvain on 29/08/18.
//

#include <X11/extensions/XTest.h>
#include <cstdio>
#include <cstdlib>
#include "X11FakeInput.h"
#include "../config.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

X11FakeInput::X11FakeInput(const char* displayName) {
    XInitThreads();
    int ignore = 0;
    this->displayName = std::getenv("DISPLAY");
    if (!this->displayName) {
        BOOST_LOG_TRIVIAL(info) << "X11FAkeInput select X11 display :0";
        this->displayName = ":0";
    }


    // open display
    if((this->display = XOpenDisplay(this->displayName)) == NULL) {
        BOOST_LOG_TRIVIAL(error) << "cannot open display " << this->displayName;
    }

    // get default screen
    this->screenNumber = XDefaultScreen(display);
    if((this->screen = XScreenOfDisplay(display, screenNumber)) == NULL) {
        BOOST_LOG_TRIVIAL(error) << "cannot obtain screen #" << this->screenNumber;
    }
    // get screen hight, width, depth
    this->width = XDisplayWidth(display, screenNumber); //TODO refactoring
    this->height = XDisplayHeight(display, screenNumber); //TODO refactoring
    int desktopDepth = XDisplayPlanes(display, screenNumber);
    BOOST_LOG_TRIVIAL(info) << "X11FakeInput X-Window-init: dimension: " << this->width << "x" << this->height << "x"
                            << "@" << desktopDepth <<  "/" << this->screenNumber;

}

void X11FakeInput::keypress_with_keycode( int keycode, int isDown )
{
    XLockDisplay(this->display);
    XTestGrabControl(this->display, True);
    XTestFakeKeyEvent(this->display,keycode, isDown ? True : False, CurrentTime);
    XFlush(this->display);
    XTestGrabControl(this->display, False);
    XUnlockDisplay(this->display);
}

void X11FakeInput::keypress_with_keysym( int keysym, int isDown )
{

    XLockDisplay(this->display);
    XTestGrabControl(this->display, True);
    XTestFakeKeyEvent(this->display,XKeysymToKeycode(this->display, keysym), isDown ? True : False, CurrentTime) ;//FIXME isdown
    XFlush(this->display);
    XTestGrabControl(this->display, False);
    XUnlockDisplay(this->display);
}

void X11FakeInput::mouseMove( float x, float y ) // TODO must be int => convertor
{
    int a = (int) this->width * x;
    int b = (int) this->height * y;
    printf("mouse x: %d, y: %d \n", a, b);
    XLockDisplay(this->display);
    XTestGrabControl(this->display, True ) ;
    XTestFakeMotionEvent(this->display, screenNumber, a, b, CurrentTime ) ;
    XFlush(this->display);
    XTestGrabControl(this->display, False ) ;
    XUnlockDisplay(this->display);
}

void X11FakeInput::mouseBtton(int button, int isDown )
{
    BOOST_LOG_TRIVIAL(debug) << "button " << button << ", is down " << isDown;
    XLockDisplay(this->display);
   XTestGrabControl(this->display, True);
    XTestFakeButtonEvent(this->display, button, isDown ? True : False, CurrentTime);
    XFlush(this->display);
    XTestGrabControl(this->display, False);
    XUnlockDisplay(this->display);
}

int X11FakeInput::getWidth() {
    return this->width;
}

int X11FakeInput::getHeight() {
    return this->height;
}


