#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifndef NETWORK_H
#define NETWORK_H

enum type {TYPE_KEY_DOWN=1, TYPE_KEY_UP=2, TYPE_MOUSE_MOTION=3, TYPE_MOUSE_DOWN=4, TYPE_MOUSE_UP=5 , TYPE_ENCODER_START=6, TYPE_ENCODER_STOP=7 };




void SRD_server_init_listen();

#endif
