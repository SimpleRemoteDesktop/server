//
// Created by sylvain on 06/09/18.
//

#include "NetworkReceiveThread.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

NetworkReceiveThread::NetworkReceiveThread(Fifo<Message> *queue, socket_ptr sock) {
    this->queue = queue;
    this->sock = sock;
    BOOST_LOG_TRIVIAL(debug) << "Creating network receive thread";
}

void NetworkReceiveThread::start() {
    boost::system::error_code error;
    this->isRunning = true;
    this->sock->set_option(tcp::no_delay(true));
    char data[sizeof(Message)];
    Message* m;
    while(this->isRunning)
    {
        size_t length = this->sock->read_some(buffer(data), error);
        if (error) {
            BOOST_LOG_TRIVIAL(info) << "connection close close";
            Message *m = new Message();
            m->type = TYPE_ENCODER_STOP;
            queue->push(m);
            throw boost::system::system_error(error);
        }
        else {
            BOOST_LOG_TRIVIAL(debug) << "receiving new message";
            m = (Message*)data;
            queue->push(m);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

    }
    BOOST_LOG_TRIVIAL(debug) << "Network receive thread exited";
}

void NetworkReceiveThread::stop() {
    this->isRunning = false;

}

NetworkReceiveThread::~NetworkReceiveThread() {
    this->stop();

}


