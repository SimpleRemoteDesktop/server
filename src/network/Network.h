//
// Created by sylvain on 06/09/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_NETWORK_H
#define SIMPLEREMOTEDESKTOP_SERVER_NETWORK_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../fifo.hpp"
#include "../config.h"
#include "NetworkReceiveThread.h"
#include "NetworkSendThread.h"


using namespace std;
using namespace boost::asio;
using ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Network {
public:
    Network(int port, Fifo<Message> *messageQueue, Fifo<SRD_Buffer_Frame> *frameQueue);
    ~Network();
    void listen();
    void stop();
    socket_ptr sock;
    Fifo<Message> *messageQueue;
    Fifo<SRD_Buffer_Frame> *frameQueue;
    NetworkReceiveThread *receiveThread;
    NetworkSendThread *sendThread;

private:
    int port;
    io_service io;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NETWORK_H
