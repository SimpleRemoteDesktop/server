//
// Created by sylvain on 29/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_X11GRAB_H
#define SIMPLEREMOTEDESKTOP_SERVER_X11GRAB_H

extern "C" { ;
#include <stdio.h>
#include<stdint.h>
#include <X11/Xlib.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

}
#include "../../Image.h"

class X11Grab {
public:
    X11Grab();
    ~X11Grab();
    Image* read();
    int width;
    int height;

private:
    XShmSegmentInfo xshminfo;
    Display *display = NULL;
    char const * displayName;
    int screenNumber;
    Screen *screen;

    XImage *ximage;
    Window rootWindow;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_X11GRAB_H
