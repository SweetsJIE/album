#ifndef __JPG_H
#define __JPG_H

#include "common.h"
#include "jpeglib.h"

struct RGB
{
	int width;
	int height;
	int bpp;
	char *rgb_data;
};
/*
struct fileinfo
{
	char name[20];
	struct RGB rgb;
	struct fileinfo *next;

};
*/

bool jpg_to_rgb(struct RGB *rgb,char *jpg_data,struct stat *jpg_info);
int read_jpg_image(int fd, char *jpg_data, int size);


#endif
