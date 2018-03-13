#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 


#ifndef MAIN_H
#define MAIN_H




#include "fifo.hpp"
#include "network.hpp"

struct Frame 
{
	unsigned char * data;
	unsigned long size;

};


struct Configuration
{
	int width;
	int height;
};

Configuration* config;

Fifo<Frame> *queueToNetwork;
Fifo<Message> *queueFromNetwork;


void handle_incoming_message(Message* m);
#endif
