#include "touch.h"

int touch_slide(int fb_ts)
{
	int x,x_begin,start_flag = 0,pressure = 0; 
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
			printf("(%d %d)\n",x,pressure);
		}
	}
	
	if(x > 0 && x > MIN_SLIDE_LEN)
		return NEXT;
	else if(x < 0 && (-1*x) > MIN_SLIDE_LEN)
		return PREV;
	else
		return 0;
}
