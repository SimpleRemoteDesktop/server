//
// Created by sylvain on 06/09/18.
//

#include "NetworkSendThread.h"

NetworkSendThread::NetworkSendThread(Fifo<SRD_Buffer_Frame> *queue, socket_ptr *sock) {
    this->queue = queue;
    this->sock = sock;
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

            boost::asio::write(this->sock, buffer(fullFrame, frame->size + 8));
            free(fullFrame);
        } else {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }


    }
}

void NetworkSendThread::stop() {
    this->isRunning = false;
}
