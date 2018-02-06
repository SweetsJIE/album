#include "common.h"


#include "touch.h"
#include "lcd.h"
#include "list.h"
#include "jpg.h"

//bool read_dir();
bool show_jpg(linklist node,char *lcd_base,int fd_lcd,int fd_jpg,struct fb_var_screeninfo vinfo)
{
	struct stat jpg_info;
	struct RGB rgb;
	
	stat(node->data,&jpg_info);

	char *jpg_data = calloc(1,jpg_info.st_size);
	fd_jpg = open(node->data,O_RDONLY);
	if(fd_jpg < 0)
	{
		perror("can not open jpg\n");
		exit(0);
	}

	read_jpg_image(fd_jpg, jpg_data, jpg_info.st_size);


	jpg_to_rgb(&rgb, jpg_data, &jpg_info);

	write_lcd_auto_match(rgb.rgb_data, &rgb, lcd_base, &vinfo,0,0);
}

int main(void)
{
	int fd_ts,fd_lcd,fd_jpg,i;
	char *lcd_base;
	
	struct dirent *ep;
	
	//初始化链表
	linklist head = init_list();
	if(head == NULL)
	{
		perror("初始化链表失败");
		exit(0);
	}

	struct stat info;
	bzero(&info, sizeof(struct stat));
	//lstat("./jpg",&info);
	
	//if(S_ISDIR(info.st_mode))
	
	DIR *dp_dir = opendir("./jpg");
	if(dp_dir == NULL)
	{
		perror("opendir() failed");
		exit(0);
	}
	chdir("./jpg");
	while(1)
	{
		ep = readdir(dp_dir);

		if(ep == NULL)
			break;
		else
		{
			lstat(ep->d_name,&info);
			if(S_ISREG(info.st_mode) && ep->d_name[0] != '.')
			{
				linklist new = new_node(ep->d_name);
				list_add_tail(new,head);
			}
		}
	}

	show(head);

	

	
	//打开触摸屏文件
	fd_ts = open("/dev/input/event0",O_RDONLY);
	if(fd_ts == -1)
	{
		perror("can not open touch file");
		exit(0);
	}
	//打开屏幕lcd文件
	fd_lcd = open("/dev/fb0",O_RDWR);
	if(fd_lcd == -1)
	{
		perror("can not open lcd file");
		exit(0);
	}

	lcd_base = mmap(NULL, 800*480*4*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd ,0);
	if(lcd_base < 0)
	{
		perror("mmap error\n");
		exit(0);
	}

	//屏幕信息结构体
	struct fb_var_screeninfo vinfo;
	ioctl(fd_lcd, FBIOGET_VSCREENINFO, &vinfo);
	
	linklist tmp = head;
	while(1)
	{
		switch(touch_slide(fd_ts))
		{
			case 1:
				tmp = tmp->next;
				if(tmp == head)
					tmp = tmp->next;
				break;
			case -1:
				tmp = tmp->prev;
				if(head == tmp)
					tmp = tmp->prev;
				break;
			default:continue;
		}
		show_jpg(tmp,lcd_base,fd_lcd,fd_jpg,vinfo);
	}

	//i = touch_slide(fb_ts);
	//printf("%d\n",i);
	
	return 0;

}
