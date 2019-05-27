//
// Created by sylvain on 20/08/18.
//

#include "SoftwareEncoder.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

SoftwareEncoder::SoftwareEncoder(int imageWidth, int imageHeight, int codecWidth, int codecHeight, int bit_rate, int fps, int pix_fmt_int ) {
	BOOST_LOG_TRIVIAL(info) << "using Software x264 encoder";
		//FFMPEG CODEC INIT

		avcodec_register_all();

		this->width = imageWidth;
		this->height = imageHeight;
		 BOOST_LOG_TRIVIAL(info) << "desktop width : " <<  this->width << " height : " << this->height;

		/* find the mpeg1 video encoder */
		codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec) {
			BOOST_LOG_TRIVIAL(info) << "Codec not found\n";
		}

		c = avcodec_alloc_context3(codec);
		if (!c) {
			BOOST_LOG_TRIVIAL(info) << "Could not allocate video codec context\n";
		}

		/* put sample parameters */
		c->bit_rate = bit_rate;
		/* resolution must be a multiple of two */
		c->width = codecWidth;
		c->height = codecHeight;
		/* frames per second */
		c->time_base = (AVRational) {1, fps};
		/* emit one intra frame every ten frames
		 * check frame pict_type before passing frame
		 * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
		 * then gop_size is ignored and the output of encoder
		 * will always be I frame irrespective to gop_size
		 */
		c->gop_size = 120;
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
			BOOST_LOG_TRIVIAL(info) << "Could not open codec\n";
		}

		frame = av_frame_alloc();
		if (!frame) {
			BOOST_LOG_TRIVIAL(info) << "Could not allocate video frame\n";
		}
		frame->format = c->pix_fmt;
		frame->width  = c->width;
		frame->height = c->height;

		/* the image can be allocated by any means and av_image_alloc() is
		 * just the most convenient way if av_malloc() is to be used */
		int ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
				c->pix_fmt, 32);
		if (ret < 0) {
			BOOST_LOG_TRIVIAL(info) << "Could not allocate raw picture buffer\n";
		}


		// initialize SWS context for software scaling
		sws_ctx = sws_getContext(
				this->width,
				this->height,
				AV_PIX_FMT_BGRA,
				//AV_PIX_FMT_RGBA, // framebuffer
				c->width,
				c->height,
				AV_PIX_FMT_YUV420P,
				SWS_FAST_BILINEAR,
				NULL,
				NULL,
				NULL
				);
}

SRD_Buffer_Frame * SoftwareEncoder::encode(Image *image) {
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;
		unsigned char* srcData[1] = {(unsigned char * ) image->data}; // convert to array of pointer for plane
		//const int inLinesize[1] = { 4 * c->width }; // bpp
		const int inLinesize[1] = { 4 * this->width }; // bpp
		//sws_scale(sws_ctx, (uint8_t const * const *) srcData,	inLinesize, 0, c->height, frame->data, frame->linesize);
		sws_scale(sws_ctx, (uint8_t const * const *) srcData, inLinesize, 0, this->height, frame->data, frame->linesize);
		frame->pts = i;
		i++;
		/* encode the image */
		int got_output;
		int ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
		if (ret < 0) {
			BOOST_LOG_TRIVIAL(info) <<"Error encoding frame\n";
			exit(1);
		}

		if (got_output) {

            SRD_Buffer_Frame *encodedFrame = new SRD_Buffer_Frame;
			encodedFrame->data = pkt.data;
			encodedFrame->size = pkt.size;
			encodedFrame->type = VIDEO_FRAME;

			return encodedFrame;
		}

}
