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
    bool socket_running = true;
        this->sock->set_option(tcp::no_delay(true));
        char data[sizeof(Message)];
        Message* m;
        while(socket_running)
        {
            size_t length = this->sock->read_some(buffer(data), error);
            if (error == error::eof) {
                std::cout << "connection close clearly must do action" << std::endl;
                return; // Connection closed cleanly by peer.

            }
            else if (error) {
                std::cout << "network error send stop command" << std::endl;
                throw boost::system::system_error(error); // Some other error.
            }
            //std::cout << "receiving new message" << std::endl;
            m = (Message*)data;
            queue->push(m);
        }
        BOOST_LOG_TRIVIAL(debug) << "Network receive thread exited";
}
