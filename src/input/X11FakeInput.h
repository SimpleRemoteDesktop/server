//
// Created by sylvain on 29/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_X11FAKEINPUT_H
#define SIMPLEREMOTEDESKTOP_SERVER_X11FAKEINPUT_H


#include <X11/Xlib.h>

class X11FakeInput {
public:
    X11FakeInput(const char* displayName);
    void keypress_with_keycode( int keycode, int isDown );
    void keypress_with_keysym( int keysym, int isDown );
    void mouseMove( float x, float y );
    void mouseBtton(int button, int isDown );
    int getWidth();
    int getHeight();

private:
    Display *display;

    int width;
    int height;
    const char *displayName;
    int screenNumber;
    Screen *screen;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_X11FAKEINPUT_H
