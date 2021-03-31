#include "include.h"
#include "ostu.h"
#include "cut.h"
#include "histogram.h"
#include "get_class_var.h"
#include "prf_thres.h"

//有多少位检测数据 
#define check_num 2

char start_char[check_num][2]={{'0','1'},{'F','E'}};
char end_char[check_num][2]={{'F','E'},{'0','1'}};

int init_sum_num=0;
uint8 image[IMAGE_H][IMAGE_W];
uint8 img[IMG_H][IMG_W];

int init()
{
	char data1,data2,kongge;
	int status=0,check_start_num_get=0,check_end_num_get=0;//0表示在查找起始标志，1表示在接受图像,2表示在接受结束字符 
	int i=0,j=0,get_pic_num=0,pic_num;
	while(~scanf("%c%c%c",&data1,&data2,&kongge))
	{
		if(status==0)
		{
			if(data1==start_char[check_start_num_get][0]&&
			data2==start_char[check_start_num_get][1])
			{
				check_start_num_get++;
				if(check_start_num_get==check_num)
				{
					status=1;
					get_pic_num=0;
				}
			}
		}
		else if(status==1)
		{
			if(data1>='0'&&data1<='9')
			{
				pic_num=(data1-'0')*16;
			}
			else
			{
				pic_num=(data1-'A'+10)*16;
			}
			if(data2>='0'&&data2<='9')
			{
				pic_num+=(data2-'0');
			}
			else
			{
				pic_num+=(data2-'A'+10);
			}
			image[get_pic_num/IMAGE_W][get_pic_num%IMAGE_W]=pic_num;
			get_pic_num++;
			if(get_pic_num==IMAGE_H*IMAGE_W)
			{
				status=2;
			}
		}
		else if(status==2)
		{
			if(data1==end_char[check_end_num_get][0]&&
			data2==end_char[check_end_num_get][1])
			{
				check_end_num_get++;
				if(check_end_num_get==check_num)
				{
					return 1;
				}
			}
			else
			{
				status=0;
				check_start_num_get=0;
				check_end_num_get=0;
			}
		}
	}
	return 0;
}

void prf_image()
{
	freopen("output_image.txt","a",stdout);
	int i,j;
	char out_1,out_2; 
	printf("01 FE ");
	for(i=0;i<IMAGE_H;i++)
	{
		for(j=0;j<IMAGE_W;j++)
		{
			if((image[i][j]/16)>9)
				out_1='A'+image[i][j]/16-10;
			else
				out_1='0'+image[i][j]/16;
			if((image[i][j]%16)>9)
				out_2='A'+image[i][j]%16-10;
			else
				out_2='0'+image[i][j]%16;
			printf("%c%c ",out_1,out_2);
		}
	}
	printf("FE 01 ");
	fclose(stdout);
}

void prf_img()
{
	freopen("output_img.txt","a",stdout);
	int i,j;
	char out_1,out_2; 
	printf("01 FE ");
	for(i=0;i<IMG_H;i++)
	{
		for(j=0;j<IMG_W;j++)
		{
			if((img[i][j]/16)>9)
				out_1='A'+img[i][j]/16-10;
			else
				out_1='0'+img[i][j]/16;
			if((img[i][j]%16)>9)
				out_2='A'+img[i][j]%16-10;
			else
				out_2='0'+img[i][j]%16;
			printf("%c%c ",out_1,out_2);
		}
	}
	printf("FE 01 ");
	fclose(stdout);
}

int main(int argc, char *argv[])
{
	freopen("input.txt","r",stdin);						//重定向输入输出流
	freopen("output_class_var.txt","w",stdout);
	freopen("output_histogram.txt","w",stdout);
	freopen("output_image.txt","w",stdout);
	freopen("output_threshold.txt","w",stdout);
	freopen("output_img.txt","w",stdout);				//用来清空txt文件
	
	int picture_num_count=0;							//处理了的图像的计数 
	int picture_num_get=0;								//如果发现某几幅图像出了问题，想单独提取出来，就把他改成非0
	int picture_num_get_end=picture_num_get+1;			//会提取出第【picture_num_get,picture_num_get_end】 幅图像 
	get_cut_array();
	while(1)
	{
		if(init())
		{
			picture_num_count++;
			if(!picture_num_get)
			{
				cut_image_to_img();
				prf_histogram();
				//prf_class_var();
				//prf_threshold();
				binary_img();
				
				///这里开始补线 
				edge_extract();				
				mid_strengthen();
				change_midline(); 				
				prf_img();
			}
			else if(picture_num_count>=picture_num_get&&picture_num_count<=picture_num_get_end)
			{
				prf_img();
				if(picture_num_count==picture_num_get_end)
					break;
			}
		}
		else
		{
			break;
		}
	}
	fclose(stdin);
	return 0;
	return 0;
}
