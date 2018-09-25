//
// Created by sylvain on 06/09/18.
//

#include "Network.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

void receiveThreadFn(Network* network)
{
   BOOST_LOG_TRIVIAL(debug) << "called receive thread fn";
    network->receiveThread = new NetworkReceiveThread(network->messageQueue, network->sock);
    network->receiveThread->start();
}

void sendThreadFn(Network* network)
{
    BOOST_LOG_TRIVIAL(debug) << "called send thread fn";
    network->sendThread = new NetworkSendThread(network->frameQueue, network->sock);
    network->sendThread->start();
}

Network::Network(int port, Fifo<Message> *messageQueue, Fifo<SRD_Buffer_Frame> *frameQueue) {
    this->port = port;
    this->messageQueue = messageQueue;
    this->frameQueue = frameQueue;

}

void Network::listen() {
    tcp::acceptor a(this->io, tcp::endpoint(tcp::v4(), this->port));
        socket_ptr sock(new tcp::socket(this->io));
        BOOST_LOG_TRIVIAL(info) << "Network : Wainting for new connection";
        a.accept(*sock);
        this->sock = sock;
        std::cout << "new socket connected" << std::endl;
        boost::thread socketReceiveThread(boost::bind(receiveThreadFn, this));
        boost::thread socketSendThread(boost::bind(sendThreadFn, this));

}
