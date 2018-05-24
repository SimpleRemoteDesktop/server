//
// Created by sylvain121 on 22/05/18.
//

#include <SDL2/SDL.h>
#include "SRD_Keyboard.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

SRD_Keyboard::SRD_Keyboard() {
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK | O_CREAT);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    ioctl(fd, UI_SET_EVBIT, EV_MSC);

    int i;
    for (i=0; i < 255; i++)
    {
        ioctl(fd, UI_SET_KEYBIT, i);
    }
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "simpleRemoteDesktop-keyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1;
    uidev.id.product = 0x1F1;
    uidev.id.version = 2;
    int ret = write(fd, &uidev, sizeof(uidev));
    if (ret < 0)
    {
        fprintf(stderr, "error while sending device  event %d\n", ret);
    }

    ret = ioctl(fd, UI_DEV_CREATE);
    if(ret < 0)
    {
        fprintf(stderr, "error while creating device %d\n", ret);
    }
    sleep(1);
    fprintf(stdout, "new keyboard device created\n");

    initKeyMap();
}



int SRD_Keyboard::press(int keycode, int isDown) {
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);

    event.type = EV_KEY;
    event.code = convertKey(keycode);
    event.value = isDown ? 1 : 0;
    fprintf(stdout, "sending keycode %d is down ? %d, sdlkeycode %d\n", event.code, event.value, keycode);
    int ret = write(fd, &event, sizeof(event));
    if(ret < 0)
    {
        fprintf(stderr, "error while sending event %d \n", ret);
    }

    memset(&event, 0, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    ret = write(fd, &event, sizeof(event));
    if(ret < 0)
    {
        fprintf(stderr, "error while sending END event %d \n", ret);
    }
}

int SRD_Keyboard::initKeyMap() {
    keysym_add(SDL_SCANCODE_BACKSPACE, KEY_BACKSPACE);
    keysym_add(SDL_SCANCODE_TAB, KEY_TAB);
    keysym_add(SDL_SCANCODE_CLEAR, KEY_CLEAR);
    keysym_add(SDL_SCANCODE_RETURN, KEY_ENTER);
    keysym_add(SDL_SCANCODE_PAUSE, KEY_PAUSE);
    keysym_add(SDL_SCANCODE_ESCAPE, KEY_ESC);
    keysym_add(SDL_SCANCODE_SPACE, KEY_SPACE);
    keysym_add(SDL_SCANCODE_MINUS, KEY_MINUS);
    keysym_add(SDL_SCANCODE_EQUALS, KEY_EQUAL);
    keysym_add(SDL_SCANCODE_Q, KEY_Q);
    keysym_add(SDL_SCANCODE_W, KEY_W);
    keysym_add(SDL_SCANCODE_E, KEY_E);
    keysym_add(SDL_SCANCODE_R, KEY_R);
    keysym_add(SDL_SCANCODE_T, KEY_T);
    keysym_add(SDL_SCANCODE_Y, KEY_Y);
    keysym_add(SDL_SCANCODE_U, KEY_U);
    keysym_add(SDL_SCANCODE_I, KEY_I);
    keysym_add(SDL_SCANCODE_O, KEY_O);
    keysym_add(SDL_SCANCODE_P, KEY_P);
    keysym_add(SDL_SCANCODE_LEFTBRACKET, KEY_LEFTBRACE);
    keysym_add(SDL_SCANCODE_RIGHTBRACKET, KEY_RIGHTBRACE);
    keysym_add(SDL_SCANCODE_A, KEY_A);
    keysym_add(SDL_SCANCODE_S, KEY_S);
    keysym_add(SDL_SCANCODE_D, KEY_D);
    keysym_add(SDL_SCANCODE_F, KEY_F);
    keysym_add(SDL_SCANCODE_G, KEY_G);
    keysym_add(SDL_SCANCODE_H, KEY_H);
    keysym_add(SDL_SCANCODE_J, KEY_J);
    keysym_add(SDL_SCANCODE_K, KEY_K);
    keysym_add(SDL_SCANCODE_L, KEY_L);
    keysym_add(SDL_SCANCODE_SEMICOLON, KEY_SEMICOLON);
    keysym_add(SDL_SCANCODE_APOSTROPHE, KEY_APOSTROPHE);
    keysym_add(SDL_SCANCODE_GRAVE, KEY_GRAVE);
    keysym_add(SDL_SCANCODE_BACKSLASH, KEY_BACKSLASH);
    keysym_add(SDL_SCANCODE_Z, KEY_Z);
    keysym_add(SDL_SCANCODE_X, KEY_X);
    keysym_add(SDL_SCANCODE_C, KEY_C);
    keysym_add(SDL_SCANCODE_V, KEY_V);
    keysym_add(SDL_SCANCODE_B, KEY_B);
    keysym_add(SDL_SCANCODE_N, KEY_N);
    keysym_add(SDL_SCANCODE_M, KEY_M);
    keysym_add(SDL_SCANCODE_COMMA, KEY_COMMA);
    keysym_add(SDL_SCANCODE_PERIOD, KEY_DOT);
    keysym_add(SDL_SCANCODE_SLASH, KEY_SLASH);
    keysym_add(SDL_SCANCODE_0, KEY_0);
    keysym_add(SDL_SCANCODE_1, KEY_1);
    keysym_add(SDL_SCANCODE_2, KEY_2);
    keysym_add(SDL_SCANCODE_3, KEY_3);
    keysym_add(SDL_SCANCODE_4, KEY_4);
    keysym_add(SDL_SCANCODE_5, KEY_5);
    keysym_add(SDL_SCANCODE_6, KEY_6);
    keysym_add(SDL_SCANCODE_7, KEY_7);
    keysym_add(SDL_SCANCODE_8, KEY_8);
    keysym_add(SDL_SCANCODE_9, KEY_9);
    keysym_add(SDL_SCANCODE_DELETE, KEY_DELETE);
    keysym_add(SDL_SCANCODE_KP_0, KEY_KP0);
    keysym_add(SDL_SCANCODE_KP_1, KEY_KP1);
    keysym_add(SDL_SCANCODE_KP_2, KEY_KP2);
    keysym_add(SDL_SCANCODE_KP_3, KEY_KP3);
    keysym_add(SDL_SCANCODE_KP_4, KEY_KP4);
    keysym_add(SDL_SCANCODE_KP_5, KEY_KP5);
    keysym_add(SDL_SCANCODE_KP_6, KEY_KP6);
    keysym_add(SDL_SCANCODE_KP_7, KEY_KP7);
    keysym_add(SDL_SCANCODE_KP_8, KEY_KP8);
    keysym_add(SDL_SCANCODE_KP_9, KEY_KP9);
    keysym_add(SDL_SCANCODE_KP_PERIOD, KEY_KPDOT);
    keysym_add(SDL_SCANCODE_KP_DIVIDE, KEY_KPSLASH);
    keysym_add(SDL_SCANCODE_KP_MULTIPLY, KEY_KPASTERISK);
    keysym_add(SDL_SCANCODE_KP_MINUS, KEY_KPMINUS);
    keysym_add(SDL_SCANCODE_KP_PLUS, KEY_KPPLUS);
    keysym_add(SDL_SCANCODE_KP_ENTER, KEY_KPENTER);
    keysym_add(SDL_SCANCODE_KP_EQUALS, KEY_KPEQUAL);
    keysym_add(SDL_SCANCODE_UP, KEY_UP);
    keysym_add(SDL_SCANCODE_DOWN, KEY_DOWN);
    keysym_add(SDL_SCANCODE_RIGHT, KEY_RIGHT);
    keysym_add(SDL_SCANCODE_LEFT, KEY_LEFT);
    keysym_add(SDL_SCANCODE_INSERT, KEY_INSERT);
    keysym_add(SDL_SCANCODE_HOME, KEY_HOME);
    keysym_add(SDL_SCANCODE_END, KEY_END);
    keysym_add(SDL_SCANCODE_PAGEUP, KEY_PAGEUP);
    keysym_add(SDL_SCANCODE_PAGEDOWN, KEY_PAGEDOWN);
    keysym_add(SDL_SCANCODE_F1, KEY_F1);
    keysym_add(SDL_SCANCODE_F2, KEY_F2);
    keysym_add(SDL_SCANCODE_F3, KEY_F3);
    keysym_add(SDL_SCANCODE_F4, KEY_F4);
    keysym_add(SDL_SCANCODE_F5, KEY_F5);
    keysym_add(SDL_SCANCODE_F6, KEY_F6);
    keysym_add(SDL_SCANCODE_F7, KEY_F7);
    keysym_add(SDL_SCANCODE_F8, KEY_F8);
    keysym_add(SDL_SCANCODE_F9, KEY_F9);
    keysym_add(SDL_SCANCODE_F10, KEY_F10);
    keysym_add(SDL_SCANCODE_F11, KEY_F11);
    keysym_add(SDL_SCANCODE_F12, KEY_F12);
    keysym_add(SDL_SCANCODE_F13, KEY_F13);
    keysym_add(SDL_SCANCODE_F14, KEY_F14);
    keysym_add(SDL_SCANCODE_F15, KEY_F15);
    keysym_add(SDL_SCANCODE_NUMLOCKCLEAR, KEY_NUMLOCK);
    keysym_add(SDL_SCANCODE_CAPSLOCK, KEY_CAPSLOCK);
    keysym_add(SDL_SCANCODE_SCROLLLOCK, KEY_SCROLLLOCK);
    keysym_add(SDL_SCANCODE_RSHIFT, KEY_RIGHTSHIFT);
    keysym_add(SDL_SCANCODE_LSHIFT, KEY_LEFTSHIFT);
    keysym_add(SDL_SCANCODE_RCTRL, KEY_RIGHTCTRL);
    keysym_add(SDL_SCANCODE_LCTRL, KEY_LEFTCTRL);
    keysym_add(SDL_SCANCODE_RALT, KEY_RIGHTALT);
    keysym_add(SDL_SCANCODE_LALT, KEY_LEFTALT);
    keysym_add(SDL_SCANCODE_RGUI, KEY_RIGHTMETA);
    keysym_add(SDL_SCANCODE_LGUI, KEY_LEFTMETA);
    keysym_add(SDL_SCANCODE_HELP, KEY_HELP);
    keysym_add(SDL_SCANCODE_SYSREQ, KEY_SYSRQ);
    keysym_add(SDL_SCANCODE_PRINTSCREEN, KEY_BREAK);
    keysym_add(SDL_SCANCODE_MENU,  KEY_MENU);
}

int SRD_Keyboard::keysym_add(int sdl, int key) {
    keymap[sdl] = key;
}


int SRD_Keyboard::convertKey(int sdlKeycode) {
    return  keymap[sdlKeycode];
}

SRD_Keyboard::~SRD_Keyboard() {
    ioctl(fd, UI_DEV_DESTROY),
    close(fd);
}
