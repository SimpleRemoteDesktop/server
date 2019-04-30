//
// Created by sylvain on 29/08/18.
//
#ifdef WITH_TJPEG
#include <vector>
#include <cstring>
#include "TJPEG_Encoder.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

TJPEG_Encoder::TJPEG_Encoder(int width, int height, int codecWidth, int codecHeight) {
    BOOST_LOG_TRIVIAL(info) << " starting TJPEG encoder";
    this->width = width;
    this->height = height;
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;
    this->isScale = !((this->width == this->codecWidth) && (this->height == this->codecHeight));

    if(this->isScale) {
        BOOST_LOG_TRIVIAL(info) << "TJPEG scaling image enable";
	//TODO
    }

    this->encoder = tjInitCompress();
    BOOST_LOG_TRIVIAL(info) << "Created TJPEG encoder instance";



}

SRD_Buffer_Frame *TJPEG_Encoder::encode(Image *image) {

    SRD_Buffer_Frame *encodedFrame = new SRD_Buffer_Frame();
    encodedFrame->type = VIDEO_FRAME;
    encodedFrame->size = 0;

    unsigned char * _c = NULL;
    long unsigned int _s = 0;

    int width = 1920;
    int height = 1080;
    unsigned char buffer[width*height*4];
    memset(buffer, 0, sizeof buffer);


     int res = tjCompress2(this->encoder,
            (unsigned char*) image->data,
	        image->width,
		    0,
		    image->height,
		    TJPF_BGRA,
		    &encodedFrame->data,
		    &encodedFrame->size,
		    TJSAMP_420,
		    95,
		   TJFLAG_FASTDCT
		);  
    if(res < 0) {
        BOOST_LOG_TRIVIAL(info) << "error" << tjGetErrorStr();
    }
    return encodedFrame;
}

#endif
