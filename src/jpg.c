#include "jpg.h"


bool jpg_to_rgb(struct RGB *rgb, char *jpg_data, struct stat *jpg_info)
{
	int i;
	// [固定步骤]声明JPG信息结构体，以及错误管理结构体
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	// [固定步骤]使用缺省的出错处理来初始化cinfo
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// [固定步骤]配置该cinfo，使其从jpg_data中读取st_size个字节
	jpeg_mem_src(&cinfo, jpg_data, jpg_info->st_size);

	// [固定步骤]读取JPG数据的头，并判断其格式是否合法
	if(jpeg_read_header(&cinfo, true) != 1)
	{
		perror("这不是JPG图片");
		return false;
	}

	// [固定步骤]开始解码
	jpeg_start_decompress(&cinfo);

	/* [注意]
	 * 此时，cinfo中存放了图片的尺寸信息：
	 * 宽：cinfo.output_width
	 * 高：cinfo.output_height
	 * 深：cinfo.output_components（单位是字节）
	 *
	 * 它们可能是你要用到的信息
	 */

	// 根据图片的尺寸大小，分配一块相应的内存rgb_data
	// 用来存放从jpg_data解码出来的RGB数据
	unsigned long rgb_size = cinfo.output_width *
				 cinfo.output_height *
				 cinfo.output_components;
	rgb->rgb_data   = calloc(1,rgb_size);
	rgb->width  = cinfo.output_width;
	rgb->height = cinfo.output_height;
	rgb->bpp    = cinfo.output_components;

	printf("w h b = %d %d %d",cinfo.output_width,cinfo.output_height,cinfo.output_components);

	// [固定步骤]循环地将图片的每一行读出并解码到rgb_data中
	int row_stride = cinfo.output_width * cinfo.output_components;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char *tmp[1];
		tmp[0] = rgb->rgb_data + (cinfo.output_scanline) * row_stride;
		jpeg_read_scanlines(&cinfo, tmp, 1);
	}

	/* [注意]
	 *
	 * 此时，rgb_data中已经存放了解码了的图像数据
	 * 它可能是你要用到的信息
	 */
	/*
	linklist head = rgb->h->next;
	for(i=0, i<800*480, i++)
	{
		head->data = ()+()+()
		head = head->next;
	}
	*/
	// 解码完毕，释放相关资源
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	
	return true;
}

int read_jpg_image(int fd, char *jpg_data, int size)
{
	int n,total = 0;
	char *tmp = jpg_data;
	while(size > 0)
	{
		n = read(fd, tmp, size);
		if(n == -1)
		{
			perror("read failed");
			exit(0);
		}
		if(n == 0)
			break;
		size -= n;
		tmp  += n;
		total += n;
	}
	return total;
}



