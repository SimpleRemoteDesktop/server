//
// Created by user on 30/03/18.
//

#include "Uinput_Keyboard.h"


Uinput_Keyboard::Uinput_Keyboard(std::string deviceName, int deviceVendor, int deviceProduct): Uinput(deviceName, deviceVendor, deviceProduct)
{

}

Uinput_Keyboard::~Uinput_Keyboard()
{

}

void Uinput_Keyboard::connect()
{
    Uinput::connect();

}