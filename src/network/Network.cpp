//
// Created by sylvain on 06/09/18.
//

#include "Network.h"
using namespace std;
using namespace boost::asio;
using ip::tcp;

Network::Network(int port) {
    this->port = port;

}

void Network::listen() {
    tcp::acceptor a(this->io, tcp::endpoint(tcp::v4(), this->port));
    for (;;)
    {
        socket_ptr sock(new tcp::socket(this->io));
        a.accept(*sock);
        std::cout << "new socket connected" << std::endl;
        boost::thread socketReceiveThead(boost::bind(new_incoming_socket_handler, sock));
        boost::thread socketSendThread(boost::bind(socket_sender_thread_fn, sock));

    }

}
