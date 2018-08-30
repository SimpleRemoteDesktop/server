//
// Created by sylvain on 09/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H
#define SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <getopt.h>
#include <linux/fb.h> /* to handle framebuffer ioctls */

#include "../../Image.h"

class FrameBufferGrab {
public:
    FrameBufferGrab();
    ~FrameBufferGrab();
    Image* read();
    int width = 0;
    int height = 0;
    int depth = 0;

private:
    void getData();
    struct fb_var_screeninfo fb_varinfo;
    size_t buf_size;
    unsigned char *buf_p;
    char const *device;
    int bits_per_pixels;
    int fd;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H
