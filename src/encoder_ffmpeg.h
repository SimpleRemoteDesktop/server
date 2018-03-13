#include "libavutil/pixfmt.h"
#include "main.hpp"
#include "xdisplay.h"

#ifdef __cplusplus
extern "C" {

#endif
void encoder_init(int *desktopWidth,int *desktopHeight,int *frameWidth,int *frameHeight, int *bit_rate, int *fps, int pix_fmt_int);
void encoder_encodeFrame(Image* image, Frame* frame);
void free_av_packet();
#ifdef __cplusplus
}
#endif
