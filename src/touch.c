#include "touch.h"
//简单检测触摸屏左右滑动
int touch_slide(int fb_ts)
{
	int x,y,x_begin,start_flag = 0,pressure = 0; 
	struct input_event ts;

	
	while(1)
	{
		
		read(fb_ts, &ts, sizeof(struct input_event));
	
		if(ts.type == EV_ABS) 
		{
			if(ts.code == ABS_X)
			{
			    	x = ts.value;
			}
			if(ts.code == ABS_Y)
			{
			    	if(ts.value < 40)
					return 2;
			}
			else if(ts.code == ABS_PRESSURE)
			{
				pressure = ts.value;
				if(pressure > 0)
				{
					start_flag =1;
					x_begin = x;
				}
				if(pressure == 0 && start_flag)
				{
					x = x - x_begin;
					break;
				}

			}
		}
	}
	
	if(x > 0 && x > MIN_SLIDE_LEN)
		return NEXT;
	else if(x < 0 && (-1*x) > MIN_SLIDE_LEN)
		return PREV;
	else
		return 0;
}
