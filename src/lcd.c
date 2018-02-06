#include "lcd.h"

/*
* @function:数据不经过任何处理直接写进lcd屏幕
* @params:
*/
void write_lcd(	char *bmp_buffer, 
		struct RGB *jpg_info,
		char *fb,
		struct fb_var_screeninfo *vinfo)
{
	bzero(fb, vinfo->xres * vinfo->yres * 4);
	int x,y;
	for(y=0; y<vinfo->yres && y<jpg_info->height; y++)
	{
		for(x=0; x<vinfo->xres && x<jpg_info->width; x++)
		{
			//if(x%xrd == 0 && xrd != 0)continue;
			unsigned long lcd_offset = (vinfo->xres * y + x) * 4;	
			unsigned long bmp_offset = (jpg_info->width * y + x) * jpg_info->bpp;

			memcpy(fb + lcd_offset + vinfo->red.offset/8,   bmp_buffer + bmp_offset + 0 ,1);
			memcpy(fb + lcd_offset + vinfo->green.offset/8, bmp_buffer + bmp_offset + 1 ,1);
			memcpy(fb + lcd_offset + vinfo->blue.offset/8,  bmp_buffer + bmp_offset + 2 ,1);
		}
	}
}

/*
* @function:图片按倍数横向或者纵向缩小
* @params:
*/
void write_lcd_reduce(	char *bmp_buffer, 
		struct RGB *jpg_info,
		char *fb,
		struct fb_var_screeninfo *vinfo,
		int xrd,
		int yrd,
		int xb,
		int yb)
{
	bzero(fb, vinfo->xres * vinfo->yres * 4);
	int x,y;
	for(y=0; y < (vinfo->yres/yrd) && y<(jpg_info->height/yrd); y++)
	{
		for(x=0; x < (vinfo->xres/xrd) && x<(jpg_info->width/xrd); x++)
		{
			unsigned long lcd_offset = (vinfo->xres * (y + yb) + (x + xb)) * 4;	
			unsigned long bmp_offset = (jpg_info->width * y * yrd + x * xrd) * jpg_info->bpp;

			memcpy(fb + lcd_offset + vinfo->red.offset/8,   bmp_buffer + bmp_offset + 0 ,1);
			memcpy(fb + lcd_offset + vinfo->green.offset/8, bmp_buffer + bmp_offset + 1 ,1);
			memcpy(fb + lcd_offset + vinfo->blue.offset/8,  bmp_buffer + bmp_offset + 2 ,1);
		}
	}
}
/*
* @function:自动匹配最适合尺寸（倍数缩小）
* @params:
*/
void write_lcd_auto_match(	char *bmp_buffer, 
				struct RGB *jpg_info,
				char *fb,
				struct fb_var_screeninfo *vinfo,
				int xb,
				int yb)	
{
	bzero(fb, vinfo->xres * vinfo->yres * 4);
	int x,y,xrd = 1, yrd = 1;
	//判断和调整图片大小
	while(1)
	{
		if(((jpg_info->width/xrd) > vinfo->xres ) || ((jpg_info->height/yrd) > vinfo->yres))
		{
			xrd++;
			yrd++;
		}
		else
			break;
	}
	//居中对其算法
	xb = vinfo->xres/2 - jpg_info->width/xrd/2;
	yb = vinfo->yres/2 - jpg_info->height/yrd/2;

	for(y=0; y<(jpg_info->height/yrd); y++)
	{
		for(x=0; x<(jpg_info->width/xrd); x++)
		{
			unsigned long lcd_offset = (vinfo->xres * (y + yb) + (x + xb)) * 4;	
			unsigned long bmp_offset = (jpg_info->width * y * yrd + x * xrd) * jpg_info->bpp;

			memcpy(fb + lcd_offset + vinfo->red.offset/8,   bmp_buffer + bmp_offset + 0 ,1);
			memcpy(fb + lcd_offset + vinfo->green.offset/8, bmp_buffer + bmp_offset + 1 ,1);
			memcpy(fb + lcd_offset + vinfo->blue.offset/8,  bmp_buffer + bmp_offset + 2 ,1);
		}
	}
}

