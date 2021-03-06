//
// Created by sylvain on 06/09/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_NETWORKRECEIVETHREAD_H
#define SIMPLEREMOTEDESKTOP_SERVER_NETWORKRECEIVETHREAD_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../fifo.hpp"
#include "../config.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class NetworkReceiveThread {
public:
    NetworkReceiveThread(Fifo<Message> *queue, socket_ptr sock);
    ~NetworkReceiveThread();
    void start();
    void stop();
private:
    Fifo<Message> *queue;
    socket_ptr sock;
    bool isRunning = false;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NETWORKRECEIVETHREAD_H
