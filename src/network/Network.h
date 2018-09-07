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

using namespace std;
using namespace boost::asio;
using ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Network {
public:
    Network(int port);
    void listen();

private:
    int port;
    io_service io;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_NETWORK_H
