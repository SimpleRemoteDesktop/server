#include "utils.h"
// FFMPEG IMPORT
#include <stdint.h>
// compatibility with newer API

#ifdef __cplusplus
extern "C" {
#endif
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixfmt.h"
#include "libavutil/samplefmt.h"
#include <libswscale/swscale.h>




#include "encoder_ffmpeg.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>

	//FFMEG vars
	AVCodec *codec;
	AVCodecContext *c = NULL;
	AVFrame *frame;
	AVPacket pkt;
	struct SwsContext *sws_ctx = NULL;
	int i = 0; //frame counter

	uint8_t *yuv[3];

	struct DesktopDimension {
		int width;
		int height;
	};
	
	struct DesktopDimension desktop;

	void encoder_init(int *desktopWidth,int *desktopHeight, int *frameWidth,int *frameHeight, int * bit_rate, int * fps, int pix_fmt_int )
	{
		
		//FFMPEG CODEC INIT

		avcodec_register_all();
	
		desktop.width = *desktopWidth;
		desktop.height = *desktopHeight;
		fprintf(stdout, "desktop width %i, height %i\n", desktop.width, desktop.height);	

		/* find the mpeg1 video encoder */
		codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec) {
			fprintf(stderr, "Codec not found\n");
			exit(1);
		}

		c = avcodec_alloc_context3(codec);
		if (!c) {
			fprintf(stderr, "Could not allocate video codec context\n");
			exit(1);
		}

		/* put sample parameters */
		c->bit_rate = *bit_rate;
		/* resolution must be a multiple of two */
		c->width = *frameWidth;
		c->height = *frameHeight;
		/* frames per second */
		c->time_base = (AVRational) {1, *fps};
		/* emit one intra frame every ten frames
		 * check frame pict_type before passing frame
		 * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
		 * then gop_size is ignored and the output of encoder
		 * will always be I frame irrespective to gop_size
		 */
		c->gop_size = 100;
		c->max_b_frames = 0;
		c->refs = 0;
		//c->pix_fmt = AV_PIX_FMT_YUV420P; //AV_PIX_FMT_YUV444P;

	switch(pix_fmt_int){
    case 1:
      c->pix_fmt = AV_PIX_FMT_YUV420P;
      break;
    case 2:
      c->pix_fmt = AV_PIX_FMT_YUV444P;
      break;
  } 

		// ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow
		av_opt_set(c->priv_data, "preset", "ultrafast", 0);
		av_opt_set(c->priv_data, "tune", "zerolatency", 0);
		//av_opt_set(c->priv_data, "profile", "baseline", 0);
		av_opt_set(c->priv_data, "slices", "4", 0);
		//c->level = 32;
			//av_opt_set(c->priv_data, "movflags", "faststart", 0);
		av_opt_set(c->priv_data, "x264opts", "no-mbtree:sliced-threads:sync-lookahead=0", 0);
		/* open it */
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			exit(1);
		}

		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			exit(1);
		}
		frame->format = c->pix_fmt;
		frame->width  = c->width;
		frame->height = c->height;

		/* the image can be allocated by any means and av_image_alloc() is
		 * just the most convenient way if av_malloc() is to be used */
		int ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
				c->pix_fmt, 32);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate raw picture buffer\n");
			exit(1);
		}


		// initialize SWS context for software scaling
		sws_ctx = sws_getContext(
				*desktopWidth,
				*desktopHeight,
				AV_PIX_FMT_BGRA,
				c->width,
				c->height,
				AV_PIX_FMT_YUV420P,
				SWS_FAST_BILINEAR,
				NULL,
				NULL,
				NULL
				);
	}
	void encoder_encodeFrame(unsigned char * srcData[], unsigned char **dstData, unsigned long *frameSize)
	{
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;

		//const int inLinesize[1] = { 4 * c->width }; // bpp
		const int inLinesize[1] = { 4 * desktop.width }; // bpp
		//sws_scale(sws_ctx, (uint8_t const * const *) srcData,	inLinesize, 0, c->height, frame->data, frame->linesize);
		sws_scale(sws_ctx, (uint8_t const * const *) srcData,	inLinesize, 0, desktop.height, frame->data, frame->linesize);
		frame->pts = i;
		i++;
		/* encode the image */
		int got_output;
		int ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
		if (ret < 0) {
			fprintf(stderr, "Error encoding frame\n");
			exit(1);
		}

		if (got_output) {

			//fprintf(stdout, "Write frame (size=%5d)\n", pkt.size);
			*dstData = pkt.data;
			*frameSize = pkt.size;
		}

	}

	void free_av_packet() {
		av_free_packet(&pkt);
	}

#ifdef __cplusplus
}
#endif
