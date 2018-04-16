//
// Created by user on 30/03/18.
//
#include <iostream>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <errno.h>

#ifndef SIMPLEREMOTEDESKTOP_SERVER_UINPUT_H
#define SIMPLEREMOTEDESKTOP_SERVER_UINPUT_H


class Uinput {
public:
    Uinput(std::string deviceName, int deviceVendor, int deviceProduct);
    ~Uinput();
    void connect();


private:
    std::string deviceName;
    int deviceProduct;
    int deviceVendor;
    uinput_user_dev uinput_device;
    int fd;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_UINPUT_H
