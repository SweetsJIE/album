#ifndef __LCD_H
#define __LCD_H

#include <linux/fb.h>
#include "common.h"
#include "jpg.h"

void write_lcd(	char *bmp_buffer, 
		struct RGB *jpg_info,
		char *fb,
		struct fb_var_screeninfo *vinfo);
void write_lcd_reduce(	char *bmp_buffer, 
			struct RGB *jpg_info,
			char *fb,
			struct fb_var_screeninfo *vinfo,
			int xrd,
			int yrd,
			int xb,
			int yb);
void write_lcd_auto_match(	char *bmp_buffer, 
				struct RGB *jpg_info,
				char *fb,
				struct fb_var_screeninfo *vinfo,
				int xb,
				int yb);	
				

#endif
