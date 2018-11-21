//
// Created by sylvain on 06/09/18.
//

#include "NetworkSendThread.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

NetworkSendThread::NetworkSendThread(Fifo<SRD_Buffer_Frame> *queue, socket_ptr sock) {
    this->queue = queue;
    this->sock = sock;
    BOOST_LOG_TRIVIAL(debug) << "Creating network send thread";
}

void NetworkSendThread::start() {

    this->isRunning = true;

    while (this->isRunning) {
        SRD_Buffer_Frame *frame = this->queue->get();
        if (frame) {
            void *fullFrame = malloc(frame->size + 8);
            memcpy(fullFrame, (void *) &frame->type, 4);
            memcpy(fullFrame + 4, (void *) &frame->size, 4);
            memcpy(fullFrame + 8, (void *) frame->data, frame->size);
            try {
                boost::asio::write(*this->sock, buffer(fullFrame, frame->size + 8));
            } catch (const std::exception &e) {
                BOOST_LOG_TRIVIAL(error) << "cannot write, TCP socket broken";
            }
            free(fullFrame);
        } else {
            boost::this_thread::sleep(boost::posix_time::milliseconds(2));
        }


    }

    BOOST_LOG_TRIVIAL(debug) << "Network send thread Exited";
}

void NetworkSendThread::stop() {
    this->isRunning = false;
}

NetworkSendThread::~NetworkSendThread() {
this->stop();
}
