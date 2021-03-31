#include "cut.h"

uint8 cut_H_start=10,cut_H_end=110;
uint8 cut_W_start=10,cut_W_end=IMAGE_W-10;

uint8 img_cut_H[IMG_H];
uint8 img_cut_W[IMG_W];

void get_cut_array()
{
	uint8 i;
	float k;
	k=(float)(cut_H_end-cut_H_start)/(float)(IMG_H-1);
	for(i=0;i<IMG_H;i++)
	{
		img_cut_H[i]=cut_H_start+k*i;
	}
	k=(float)(cut_W_end-cut_W_start)/(float)(IMG_W-1);
	for(i=0;i<IMG_W;i++)
	{
		img_cut_W[i]=cut_W_start+k*i;
	}
}

void cut_image_to_img()
{
	uint8 i,j;
	for(i=0;i<IMG_H;i++)
	{
		for(j=0;j<IMG_W;j++)
		{
			img[i][j]=image[img_cut_H[i]][img_cut_W[j]];
		}
	}
}
