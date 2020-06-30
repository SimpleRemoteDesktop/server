//
// Created by sylvain on 10/08/18.
//

#include "SRD_Touchscreen.h"
//
// Created by sylvain on 09/08/18.
//

#include "SRD_Mouse.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

SRD_Touchscreen::SRD_Touchscreen(int desktopWidth, int desktopHeight) {

	this->width = desktopWidth;
	this->height = desktopHeight;
	int version, rc;

	fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0)
		fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if(fd < 0)
		fprintf(stderr, "error: open \"/dev/input/uinput\" and \"/dev/uinput\"");

	ioctl(fd, UI_GET_VERSION, &version);

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_EVBIT, EV_SYN);
	ioctl(fd, UI_SET_EVBIT, EV_MSC);

	ioctl(fd, UI_SET_EVBIT, EV_ABS);
	ioctl(fd, UI_SET_ABSBIT, ABS_X);
	ioctl(fd, UI_SET_ABSBIT, ABS_Y);

//	ioctl(fd, UI_SET_KEYBIT, BTN_DIGI); // Indicates this is an absolute mouse
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);
//	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

	struct uinput_abs_setup abs_setup;
	
	memset(&abs_setup, 0, sizeof(abs_setup));
        abs_setup.code = ABS_X;
        abs_setup.absinfo.value = 0;
        abs_setup.absinfo.minimum = 0;
        abs_setup.absinfo.maximum = UINT16_MAX;
        abs_setup.absinfo.fuzz = 0;
        abs_setup.absinfo.flat = 0;
       // abs_setup.absinfo.resolution = 400;
        if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
		printf("error: UI_ABS_SETUP ABS_X");
        
	memset(&abs_setup, 0, sizeof(abs_setup));
        abs_setup.code = ABS_Y;
        abs_setup.absinfo.value = 0;
        abs_setup.absinfo.minimum = 0;
        abs_setup.absinfo.maximum = UINT16_MAX;
        abs_setup.absinfo.fuzz = 0;
        abs_setup.absinfo.flat = 0;
        //abs_setup.absinfo.resolution = 400;
        if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
		printf("error: UI_ABS_SETUP ABS_Y");

        memset(&abs_setup, 0, sizeof(abs_setup));
        abs_setup.code = ABS_PRESSURE;
        abs_setup.absinfo.value = 0;
        abs_setup.absinfo.minimum = 0;
        abs_setup.absinfo.maximum = INT16_MAX;
        abs_setup.absinfo.fuzz = 0;
        abs_setup.absinfo.flat = 0;
        abs_setup.absinfo.resolution = 0;
        if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
	 	printf("error: UI_ABS_SETUP ABS_PRESSURE");

	struct uinput_setup usetup;
	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0x1;
	usetup.id.product = 0x1F5;
	strcpy(usetup.name, "SRD Touchscreen input driver uinput api v5");

	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);
}

int SRD_Touchscreen::mouseButton(int button, int isDown) {
	memset(&ev, 0, sizeof(ev));
	gettimeofday(&ev.time, NULL);
	BOOST_LOG_TRIVIAL(debug) << "mouse button :" << button << " isDown: "  << isDown;
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

int SRD_Touchscreen::mouseMove(float  fx, float fy) {
	int x = (int) this->width * fx;
	int y = (int) this->height * fy;
	memset(&ev, 0, sizeof(struct input_event));
	gettimeofday(&ev.time, NULL);
	ev.type = EV_ABS;
	ev.code = ABS_X;
	ev.value = x;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		fprintf(stderr, "error: write");

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_ABS;
	ev.code = ABS_Y;
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
