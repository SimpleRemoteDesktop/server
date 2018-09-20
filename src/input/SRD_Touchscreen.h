//
// Created by sylvain on 10/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_SRD_TOUCHSCREEN_H
#define SIMPLEREMOTEDESKTOP_SERVER_SRD_TOUCHSCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

class SRD_Touchscreen {
public:
    SRD_Touchscreen(int desktopWidth, int desktopHeight);
    ~SRD_Touchscreen();
    int mouseMove(float fx, float fy);
    int mouseButton(int button, int isDown);
private:
    struct input_event ev;
    int fd;
    int width;
    int height;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_SRD_TOUCHSCREEN_H
