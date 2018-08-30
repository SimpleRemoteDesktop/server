//
// Created by sylvain on 20/08/18.
//
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SIMPLEREMOTEDESKTOP_SERVER_IMAGE_H
#define SIMPLEREMOTEDESKTOP_SERVER_IMAGE_H

	enum pixel_format { RGBA32, BGRA32};
	enum subsampling { YUV_420P, YUV_444P };

	typedef struct {
		int pixel_format;
		int width;
		int height;
		char * data;
		int depth;
		int size;
		int bytes_per_line;
		int bits_per_pixel;

	} Image;
#endif //SIMPLEREMOTEDESKTOP_SERVER_IMAGE_H
#ifdef __cplusplus
}
#endif