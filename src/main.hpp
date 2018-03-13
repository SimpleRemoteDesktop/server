#ifndef MAIN_H
#define MAIN_H

#include "fifo.hpp"
#include "network.hpp"
#include "encoder_ffmpeg.h"
#include "config.h"


extern Fifo<Frame> *queueToNetwork;
extern Fifo<Message> *queueFromNetwork;


void handle_incoming_message(Message* m);
#endif
