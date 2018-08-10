//
// Created by sylvain on 09/08/18.
//

#include "SRD_Mouse.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

SRD_Mouse::SRD_Mouse() {

	struct uinput_user_dev uidev;

	fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0)
		fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if(fd < 0)
		fprintf(stderr, "error: open \"/dev/input/uinput\" and \"/dev/uinput\"");

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_EVBIT, EV_SYN);
	ioctl(fd, UI_SET_EVBIT, EV_MSC);


	ioctl(fd, UI_SET_EVBIT, EV_REL);
	ioctl(fd, UI_SET_RELBIT, REL_X);
	ioctl(fd, UI_SET_RELBIT, REL_Y);
	ioctl(fd, UI_SET_RELBIT, REL_WHEEL);

	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "SRD mouse input");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1F2;
	uidev.id.version = 2;

	if(write(fd, &uidev, sizeof(uidev)) < 0)
		fprintf(stderr, "error: write &uidev");

	if(ioctl(fd, UI_DEV_CREATE) < 0)
		fprintf(stderr, "error: ioctl UI_DEV_CREATE");
}

int SRD_Mouse::mouseButton(int button, int isDown) {
	memset(&ev, 0, sizeof(ev));
	gettimeofday(&ev.time, NULL);
	BOOST_LOG_TRIVIAL(info) << "mouse button :" << button << " isDown: "  << isDown;
	ev.type = EV_KEY;
	switch(button) {
		case 1:
			ev.code = BTN_LEFT;
			break;
		case 2:
			ev.code = BTN_RIGHT;
	}
	ev.value = isDown;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");
	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");
}

int SRD_Mouse::mouseMove(int x, int y) {
	memset(&ev, 0, sizeof(struct input_event));
	gettimeofday(&ev.time, NULL);
	ev.type = EV_REL;
	ev.code = REL_X;
	ev.value = x;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_REL;
	ev.code = REL_Y;
	ev.value = y;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");
}
