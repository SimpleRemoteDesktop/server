//
// Created by sylvain on 22/05/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_KEYBOARD_H
#define SIMPLEREMOTEDESKTOP_SERVER_KEYBOARD_H
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <map>

class SRD_Keyboard {
public:
    SRD_Keyboard();
    ~SRD_Keyboard();
    int press(int keycode, int isDown);
private:
    int initKeyMap();
    int keysym_add(int sdl, int key);
    int convertKey(int sdlKeycode);
    input_event event;
    std::map<int, int> keymap;
    uinput_user_dev uidev;
    int fd;
};


#endif //SIMPLEREMOTEDESKTOP_SERVER_KEYBOARD_H
