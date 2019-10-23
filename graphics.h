#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct BMP_Picture{
	unsigned char header[54];
	unsigned int width;
	unsigned int height;
	//Image size 
	unsigned int size;
	unsigned int data_start;
	//RGB data:
	unsigned char* data;
};


struct BMP_Picture* load_bmp_image(const char* name);
void save_bmp_image(struct BMP_Picture* image, const char* name);
void free_bmp_image(struct BMP_Picture* image);

//Converting function - using mogrify
void convert_to_bmp(const char* format, const char* name); //Format: "jpg", "png"

#endif
