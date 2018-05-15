//
// Created by sylvain121 on 30/03/18.
//
#include <stdio.h>
#include <string.h>
#include "Uinput.h"





Uinput::Uinput(std::string deviceName, int deviceVendor, int deviceProduct ) {
    this->deviceName = deviceName;
    this->deviceVendor = deviceVendor;
    this->deviceProduct = deviceProduct;


}

Uinput::~Uinput() {
}

void Uinput::connect() {
    fd = open("/dev/uinput", O_RDWR);
    // if -1 TODO throw error
    memset(&uinput_device, 0, sizeof(uinput_user_dev));
    strncpy(uinput_device.name, this->deviceName.c_str(), 20);
    uinput_device.id.version = 4;
    uinput_device.id.bustype = BUS_USB;
}

extern "C" void uinput_setup(uinput_user_dev* uinp, char* deviceName) {

}


