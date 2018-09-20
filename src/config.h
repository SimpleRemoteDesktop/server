#ifndef CONFIG_H
#define CONFIG_H


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
	float x;
	float y;
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
