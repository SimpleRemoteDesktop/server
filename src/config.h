#ifndef CONFIG_H
#define CONFIG_H

enum type {TYPE_KEY_DOWN=1, TYPE_KEY_UP=2, TYPE_MOUSE_MOTION=3, TYPE_MOUSE_DOWN=4, TYPE_MOUSE_UP=5 , TYPE_ENCODER_START=6, TYPE_ENCODER_STOP=7 };

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    unsigned char * data;
    unsigned long int size;
    int type;

} SRD_Buffer_Frame;

typedef struct
{
	int type;
	int x;
	int y;
	int button;
	int keycode;
	int scancode;
	int codec_width;
	int codec_height;
	int bandwidth;
	int fps;
	int sdl;

} Message;

#define VIDEO_FRAME 1
#define SRD_AUDIO_FRAME 2

	typedef struct
	{
		int width;
		int height;
	} Configuration;

	extern 	Configuration* config;
#ifdef __cplusplus
}
#endif
#endif
