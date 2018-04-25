#ifndef MAIN_H
#define MAIN_H

#include "fifo.hpp"

#include "config.h"
#include "sound/SoundManager.h"


extern Fifo<SRD_Buffer_Frame> *queueToNetwork;
extern Fifo<Message> *queueFromNetwork;


void handle_incoming_message(Message* m);
#endif
