//
// Created by user on 30/03/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_UINPUT_KEYBOARD_H
#define SIMPLEREMOTEDESKTOP_SERVER_UINPUT_KEYBOARD_H

#include "Uinput.h"

class Uinput_Keyboard: public Uinput  {
public:
    Uinput_Keyboard(std::string deviceName, int deviceVendor, int deviceProduct);
    ~Uinput_Keyboard();
    void connect();

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_UINPUT_KEYBOARD_H
