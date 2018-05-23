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
    keysym_add(SDLK_BACKSPACE, KEY_BACKSPACE);
    keysym_add(SDLK_TAB, KEY_TAB);
    keysym_add(SDLK_CLEAR, KEY_CLEAR);
    keysym_add(SDLK_RETURN, KEY_ENTER);
    keysym_add(SDLK_PAUSE, KEY_PAUSE);
    keysym_add(SDLK_ESCAPE, KEY_ESC);
    keysym_add(SDLK_SPACE, KEY_SPACE);
    keysym_add(SDLK_MINUS, KEY_MINUS);
    keysym_add(SDLK_EQUALS, KEY_EQUAL);
    keysym_add(SDLK_q, KEY_Q);
    keysym_add(SDLK_w, KEY_W);
    keysym_add(SDLK_e, KEY_E);
    keysym_add(SDLK_r, KEY_R);
    keysym_add(SDLK_t, KEY_T);
    keysym_add(SDLK_y, KEY_Y);
    keysym_add(SDLK_u, KEY_U);
    keysym_add(SDLK_i, KEY_I);
    keysym_add(SDLK_o, KEY_O);
    keysym_add(SDLK_p, KEY_P);
    keysym_add(SDLK_LEFTBRACKET, KEY_LEFTBRACE);
    keysym_add(SDLK_RIGHTBRACKET, KEY_RIGHTBRACE);
    keysym_add(SDLK_a, KEY_A);
    keysym_add(SDLK_s, KEY_S);
    keysym_add(SDLK_d, KEY_D);
    keysym_add(SDLK_f, KEY_F);
    keysym_add(SDLK_g, KEY_G);
    keysym_add(SDLK_h, KEY_H);
    keysym_add(SDLK_j, KEY_J);
    keysym_add(SDLK_k, KEY_K);
    keysym_add(SDLK_l, KEY_L);
    keysym_add(SDLK_SEMICOLON, KEY_SEMICOLON);
    keysym_add(SDLK_QUOTE, KEY_APOSTROPHE);
    keysym_add(SDLK_BACKQUOTE, KEY_GRAVE);
    keysym_add(SDLK_BACKSLASH, KEY_BACKSLASH);
    keysym_add(SDLK_z, KEY_Z);
    keysym_add(SDLK_x, KEY_X);
    keysym_add(SDLK_c, KEY_C);
    keysym_add(SDLK_v, KEY_V);
    keysym_add(SDLK_b, KEY_B);
    keysym_add(SDLK_n, KEY_N);
    keysym_add(SDLK_m, KEY_M);
    keysym_add(SDLK_COMMA, KEY_COMMA);
    keysym_add(SDLK_PERIOD, KEY_DOT);
    keysym_add(SDLK_SLASH, KEY_SLASH);
    keysym_add(SDLK_0, KEY_0);
    keysym_add(SDLK_1, KEY_1);
    keysym_add(SDLK_2, KEY_2);
    keysym_add(SDLK_3, KEY_3);
    keysym_add(SDLK_4, KEY_4);
    keysym_add(SDLK_5, KEY_5);
    keysym_add(SDLK_6, KEY_6);
    keysym_add(SDLK_7, KEY_7);
    keysym_add(SDLK_8, KEY_8);
    keysym_add(SDLK_9, KEY_9);
    keysym_add(SDLK_1, KEY_1);
    keysym_add(SDLK_1, KEY_1);
    keysym_add(SDLK_DELETE, KEY_DELETE);
    keysym_add(SDLK_KP_0, KEY_KP0);
    keysym_add(SDLK_KP_1, KEY_KP1);
    keysym_add(SDLK_KP_2, KEY_KP2);
    keysym_add(SDLK_KP_3, KEY_KP3);
    keysym_add(SDLK_KP_4, KEY_KP4);
    keysym_add(SDLK_KP_5, KEY_KP5);
    keysym_add(SDLK_KP_6, KEY_KP6);
    keysym_add(SDLK_KP_7, KEY_KP7);
    keysym_add(SDLK_KP_8, KEY_KP8);
    keysym_add(SDLK_KP_9, KEY_KP9);
    keysym_add(SDLK_KP_PERIOD, KEY_KPDOT);
    keysym_add(SDLK_KP_DIVIDE, KEY_KPSLASH);
    keysym_add(SDLK_KP_MULTIPLY, KEY_KPASTERISK);
    keysym_add(SDLK_KP_MINUS, KEY_KPMINUS);
    keysym_add(SDLK_KP_PLUS, KEY_KPPLUS);
    keysym_add(SDLK_KP_ENTER, KEY_KPENTER);
    keysym_add(SDLK_KP_EQUALS, KEY_KPEQUAL);
    keysym_add(SDLK_UP, KEY_UP);
    keysym_add(SDLK_DOWN, KEY_DOWN);
    keysym_add(SDLK_RIGHT, KEY_RIGHT);
    keysym_add(SDLK_LEFT, KEY_LEFT);
    keysym_add(SDLK_INSERT, KEY_INSERT);
    keysym_add(SDLK_HOME, KEY_HOME);
    keysym_add(SDLK_END, KEY_END);
    keysym_add(SDLK_PAGEUP, KEY_PAGEUP);
    keysym_add(SDLK_PAGEDOWN, KEY_PAGEDOWN);
    keysym_add(SDLK_F1, KEY_F1);
    keysym_add(SDLK_F2, KEY_F2);
    keysym_add(SDLK_F3, KEY_F3);
    keysym_add(SDLK_F4, KEY_F4);
    keysym_add(SDLK_F5, KEY_F5);
    keysym_add(SDLK_F6, KEY_F6);
    keysym_add(SDLK_F7, KEY_F7);
    keysym_add(SDLK_F8, KEY_F8);
    keysym_add(SDLK_F9, KEY_F9);
    keysym_add(SDLK_F10, KEY_F10);
    keysym_add(SDLK_F11, KEY_F11);
    keysym_add(SDLK_F12, KEY_F12);
    keysym_add(SDLK_F13, KEY_F13);
    keysym_add(SDLK_F14, KEY_F14);
    keysym_add(SDLK_F15, KEY_F15);
    keysym_add(SDLK_NUMLOCKCLEAR, KEY_NUMLOCK);
    keysym_add(SDLK_CAPSLOCK, KEY_CAPSLOCK);
    keysym_add(SDLK_SCROLLLOCK, KEY_SCROLLLOCK);
    keysym_add(SDLK_RSHIFT, KEY_RIGHTSHIFT);
    keysym_add(SDLK_LSHIFT, KEY_LEFTSHIFT);
    keysym_add(SDLK_RCTRL, KEY_RIGHTCTRL);
    keysym_add(SDLK_LCTRL, KEY_LEFTCTRL);
    keysym_add(SDLK_RALT, KEY_RIGHTALT);
    keysym_add(SDLK_LALT, KEY_LEFTALT);
    keysym_add(SDLK_RGUI, KEY_RIGHTMETA);
    keysym_add(SDLK_LGUI, KEY_LEFTMETA);
    keysym_add(SDLK_HELP, KEY_HELP);
    keysym_add(SDLK_SYSREQ, KEY_SYSRQ);
    keysym_add(SDLK_PRINTSCREEN, KEY_BREAK);
    keysym_add(SDLK_MENU,  KEY_MENU);
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
