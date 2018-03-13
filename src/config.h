#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
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
