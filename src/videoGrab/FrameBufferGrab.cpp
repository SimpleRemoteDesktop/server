//
// Created by sylvain on 09/08/18.
//

#include "FrameBufferGrab.h"
#define	DEFAULT_FB	"/dev/fb0"
#define UNDEFINED -1

FrameBufferGrab::FrameBufferGrab() {
    if (NULL == device)
    {
        device = getenv("FRAMEBUFFER");
        if (NULL == device)
        {
            device = DEFAULT_FB;
        }
    }

    this.get_framebufferdata(device, &fb_varinfo);


    if (UNDEFINED == bitdepth)
        bitdepth = (int) fb_varinfo.bits_per_pixel;

    if (UNDEFINED == width)
        width = (int) fb_varinfo.xres;

    if (UNDEFINED == height)
        height = (int) fb_varinfo.yres;

    buf_size = width * height * (((unsigned int) bitdepth + 7) >> 3);

    buf_p = malloc(buf_size);

    if(buf_p == NULL)
        fatal_error("Not enough memory");

    memset(buf_p, 0, buf_size);
}

void FrameBufferGrab::get_framebufferdata(char *device, struct fb_var_screeninfo *fb_varinfo_p)
{
    int fd;

    /* now open framebuffer device */
    if(-1 == (fd=open(device, O_RDONLY)))
    {
        fprintf (stderr, "Error: Couldn't open %s.\n", device);
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, fb_varinfo_p) != 0)
        fatal_error("ioctl FBIOGET_VSCREENINFO");

    (void) close(fd);
}

void FrameBufferGrab::read_framebuffer(size_t bytes, unsigned char *buf_p)
{
    int fd;

    if(-1 == (fd=open(device, O_RDONLY)))
    {
        fprintf (stderr, "Error: Couldn't open %s.\n", device);
        exit(EXIT_FAILURE); //FIXME
    }

    if (buf_p == NULL || read(fd, buf_p, bytes) != (ssize_t) bytes)
        fatal_error("Error: Not enough memory or data\n"); //FIXME
}