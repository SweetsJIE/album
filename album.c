#include "common.h"


#include "touch.h"
#include "lcd.h"
#include "list.h"
#include "jpg.h"
//读取./jpg目录下的jpg和jpeg文件
bool read_dir(linklist head)
{	
	struct stat info;
	struct dirent *ep;
	bzero(&info, sizeof(struct stat));
	
	//打开目录文件
	DIR *dp_dir = opendir("./jpg");
	if(dp_dir == NULL)
	{
		perror("opendir() failed");
		exit(0);
	}
	//进入目标目录
	chdir("./jpg");
	while(1)
	{
		//读取目录下的文件名字
		ep = readdir(dp_dir);

		if(ep == NULL)
			break;
		else
		{
			//获取文件属性
			lstat(ep->d_name,&info);
			//判断普通文件和jpg、jpeg文件
			if(S_ISREG(info.st_mode) && ep->d_name[0] != '.' && 
			   ( strstr(ep->d_name,".jpg") != NULL || strstr(ep->d_name, ".jpeg") !=NULL ))
			{
				linklist new = new_node(ep->d_name);
				list_add_tail(new,head);
			}
		}
	}
}
//读取jpg数据解析放进rgb结构体里面
void read_jpg(linklist node)
{
	int fd_jpg;
	struct stat jpg_info;
	//获取链表中文件属性
	stat(node->data,&jpg_info);
	
	//读取jpg文件
	char *jpg_data = calloc(1,jpg_info.st_size);
	fd_jpg = open(node->data,O_RDONLY);
	if(fd_jpg < 0)
	{
		perror("can not open jpg\n");
		exit(0);
	}

	//读取jpg文件数据
	read_jpg_image(fd_jpg, jpg_data, jpg_info.st_size);
	//开始解码图片
	jpg_to_rgb(node->rgb, jpg_data, &jpg_info);

	close(fd_jpg);
}
//链表中rgb数据缓存
void jpg_cache(linklist current ,int direction)
{
	static int times = 0;
	linklist p;

	//链表中rgb重定向
	if(direction == NEXT)
	{
		current->next->rgb = current->prev->prev->rgb;
		current->prev->prev->rgb = NULL;
		p = current->next;
	}
	else if(direction == PREV)
	{
		current->prev->rgb = current->next->next->rgb;
		current->next->next->rgb = NULL;
		p = current->prev;
	}
	if(times == 0)//第一次读取前中后三张照片
	{
		read_jpg(current);
		read_jpg(current->next);
		read_jpg(current->prev);
		times = 1;
	}
	else
		read_jpg(p);

	
}


//显示图片（先写进另外一部分屏幕虚拟内存再显示出来）
bool show_jpg(linklist node,char *lcd_base,int fd_lcd,struct fb_var_screeninfo vinfo)
{

	static int k = -1;
	k = (k + 1)%2;
	
	//屏幕大小
	int scr_size = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
	//数据写入虚拟缓冲区
	write_lcd_auto_match(node->rgb->rgb_data, node->rgb, lcd_base + k*scr_size , &vinfo,0,0);
	//改变偏移量
	vinfo.xoffset = 0;
	vinfo.yoffset = k*vinfo.yres;
	if(ioctl(fd_lcd, FBIOPAN_DISPLAY, &vinfo) == -1)
	{
		perror("ioctl FBIOPAN_DISPLAY failed");
		return;
	}
	//释放jpg_data?
	
}

int main(void)
{
	int fd_ts,fd_lcd,i,direction;
	char *lcd_base;
	
	
	//初始化链表
	linklist head = init_list();
	if(head == NULL)
	{
		perror("初始化链表失败");
		exit(0);
	}


 	read_dir(head);
	

	
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

	//地址重映射
	lcd_base = mmap(NULL, 800*480*4*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd ,0);
	if(lcd_base < 0)
	{
		perror("mmap error\n");
		exit(0);
	}
	
	//获取lcd屏幕信息
	struct fb_var_screeninfo vinfo;
	ioctl(fd_lcd, FBIOGET_VSCREENINFO, &vinfo);
	
	//暴力删除头节点
	head->prev->next = head->next;
	head->next->prev = head->prev;

	head = head->next;
	linklist current = head;
	//申请三块内存存放rgb数据
	head->rgb = calloc(1,sizeof(struct RGB));
	head->next->rgb = calloc(1,sizeof(struct RGB));
	head->prev->rgb = calloc(1,sizeof(struct RGB));

	jpg_cache(head,0);
	while(1)
	{
		direction = touch_slide(fd_ts);
		if(direction == OVER)
			break;
		switch(direction)
		{
			case NEXT:
				current = current->next;
				break;
			case PREV:
				current = current->prev;
				break;
			default:continue;
		}
		show_jpg(current,lcd_base,fd_lcd,vinfo);
		jpg_cache(current,direction);
	}

	close(fd_lcd);
	close(fd_ts);
	munmap(lcd_base,800*480*4*2);

	return 0;

}
