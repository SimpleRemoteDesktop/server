//
// Created by sylvain on 09/08/18.
//

#include "SRD_Mouse.h"


SRD_Mouse::SRD_Mouse() {
    int fd;
    struct uinput_user_dev uidev;

    fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if(fd < 0)
        errexit("error: open \"/dev/input/uinput\" and \"/dev/uinput\"");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        errexit("error: ioctl(fd, UI_SET_EVBIT, EV_KEY)");
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        errexit("error: ioctl(fd, UI_SET_KEYBIT, BTN_LEFT)");

    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        errexit("error: ioctl(fd, UI_SET_EVBIT, EV_REL)");
    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
        errexit("error: ioctl(fd, UI_SET_RELBIT, REL_X)");
    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        errexit("error: ioctl(fd, UI_SET_RELBIT, REL_Y)");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        errexit("error: write &uidev");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        errexit("error: ioctl UI_DEV_CREATE");

    sleep(1);

    struct input_event ev;
    int i, dx, dy;
}

int SRD_Mouse::mouseButton() {

}

int SRD_Mouse::mouseMove(int x, int y) {
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_REL;
    ev.code = REL_X;
    ev.value = dx;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        errexit("error: write");

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_REL;
    ev.code = REL_Y;
    ev.value = dy;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        errexit("error: write");

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        errexit("error: write");
}
