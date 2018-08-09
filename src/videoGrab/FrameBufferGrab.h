//
// Created by sylvain on 09/08/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H
#define SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H


class FrameBufferGrab {
    FrameBufferGrab();
    ~FrameBufferGrab();

private:
    void FrameBufferGrab::get_framebufferdata(char *device, struct fb_var_screeninfo *fb_varinfo_p);
    struct fb_var_screeninfo fb_varinfo;
    size_t buf_size;
    unsigned char *buf_p;
    char *device;
    int width;
    int height;
    int depth;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_FRAMEBUFFERGRAB_H
