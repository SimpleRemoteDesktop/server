//
// Created by sylvain on 23/08/18.
//

#include "VideoCapture.h"

void videoCapture_running_thread_fn(VideoCapture* videoCapture) {
    videoCapture->capture();
}

VideoCapture::VideoCapture(int codecWidth, int codecHeight, int bit_rate, int fps, Fifo<SRD_Buffer_Frame> *queueToNetwork, bool withNvEnc) {

    this->outputQueue = queueToNetwork;
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;
    this->bit_rate = bit_rate;
    this->fps = fps;
    this->withNvEnc = withNvEnc;

    this->duration = (float) 1000 / this->fps;

    //this->grab = new FrameBufferGrab();
    this->grab = new X11Grab();
    if(this->withNvEnc) {
        this->encoder  = new NVENC_Encoder(this->grab->width, this->grab->height, this->codecWidth, this->codecHeight, this->bit_rate, this->fps);
    } else {
        this->encoder = new SoftwareEncoder(this->grab->width, this->grab->height, this->codecWidth, this->codecHeight, this->bit_rate, this->fps, 1);
    }

}

void VideoCapture::start() {
    this->isRunningThread = true;
    boost::thread socketSendThread(boost::bind(videoCapture_running_thread_fn, this));
}

void VideoCapture::stop() {
    BOOST_LOG_TRIVIAL(info) << "Stopping video thread";
    this->isRunningThread = false;
}

void VideoCapture::capture() {
    BOOST_LOG_TRIVIAL(info) << "Starting new video thread with loop duration : " << duration << " ms";
    BOOST_LOG_TRIVIAL(debug) << "is Running ? " << this->isRunningThread;
    while(this->isRunningThread)
    {
        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

        BOOST_LOG_TRIVIAL(debug) << "getting X11 image";
        Image* image = this->grab->read();

        BOOST_LOG_TRIVIAL(debug) << "sending to encoder";
        SRD_Buffer_Frame* frame = this->encoder->encode(image);
        free(image->data);
        free(image);
        this->outputQueue->push(frame);

        boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = t2 - t1;
        BOOST_LOG_TRIVIAL(debug) << "encoder time : " << diff.total_milliseconds();

        boost::this_thread::sleep(boost::posix_time::milliseconds(duration - diff.total_milliseconds()));
    }


}
