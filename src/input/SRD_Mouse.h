//
// Created by sylvain on 09/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_SRD_MOUSE_H
#define SIMPLEREMOTEDESKTOP_SERVER_SRD_MOUSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

class SRD_Mouse {
public:
    SRD_Mouse();
    ~SRD_Mouse();
    int mouseMove(int x, int y);
    int mouseButton(int button, int isDown);
private:
    struct input_event ev;
    int fd;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_SRD_MOUSE_H
