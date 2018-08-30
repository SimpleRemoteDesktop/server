//
// Created by sylvain on 09/08/18.
//
#include "FrameBufferGrab.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#define    DEFAULT_FB    "/dev/fb0"

FrameBufferGrab::FrameBufferGrab() {
    if (!device) {
        device = getenv("FRAMEBUFFER");
        if (!device) {
            device = DEFAULT_FB;
        }
    }

    BOOST_LOG_TRIVIAL(info) << "try to open frame buffer " << device << std::endl;
    this->fd = open(device, O_RDONLY);
    if ( this->fd == -1) {
        BOOST_LOG_TRIVIAL(error) << "Error: Couldn't open " << device << std::endl;
        throw std::string("could not open device");
    }

    this->getData();
    this->bits_per_pixels = (int) fb_varinfo.bits_per_pixel;
    this->width = (int) fb_varinfo.xres;
    this->height = (int) fb_varinfo.yres;
    this->depth = (((unsigned int) this->bits_per_pixels + 7) >> 3);
    this->buf_size = this->width * this->height * this->depth;
}

void FrameBufferGrab::getData() {

    memset(&this->fb_varinfo, 0, sizeof(struct fb_var_screeninfo));
    int ret = ioctl(this->fd, FBIOGET_VSCREENINFO, &this->fb_varinfo);
    if ( ret != 0)
        BOOST_LOG_TRIVIAL(error) << "ioctl FBIOGET_VSCREENINFO : " << ret << std::endl;

}

Image* FrameBufferGrab::read() {

    Image *image = new Image();
    image->depth = this->depth;
    image->height = this->height;
    image->width = this->width;
    image->bits_per_pixel = this->bits_per_pixels;
    image->size = this->buf_size;
    image->data = (char *) malloc(image->size);
    memset(image->data, 0, image->size);
    ::read(this->fd, image->data, image->size);

    return image;
}

FrameBufferGrab::~FrameBufferGrab() {
    (void) close(fd);
}
