#ifndef __TOUCH_H
#define __TOUCH_H

#include "common.h"
#include <linux/input.h>

#define MIN_SLIDE_LEN 300
#define NEXT 1
#define PREV -1
#define OVER 2

int touch_slide(int fb_ts);

#endif
