#include "histogram.h"

#define Step   4							//仅限1,2,4,8等2的次方 
#define Step_yu    (256-Step)				//4步   等差  灰度级

uint16 img_histogram[256];

void get_histogram()
{
	uint8 i,j;
	
	uint8 height_start=10;
	uint8 width_start=0;
	
	uint8  width = IMG_W;  
	uint8  height = IMG_H;
	
	memset(img_histogram,0,sizeof(img_histogram));
	
	//做出灰度直方图 
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			img_histogram[img[i][j]&Step_yu]++;
		}
	}
}

void prf_histogram()
{
	freopen("output_histogram.txt","a",stdout);
	get_histogram();
	int i;
	for(i=0;i<256;i++)
	{
		if(img_histogram[i]>=1000)
		{
			printf("%c",'0'+img_histogram[i]/1000);
		}
		if(img_histogram[i]>=100)
		{
			printf("%c",'0'+(img_histogram[i]%1000)/100);
		}
		if(img_histogram[i]>=10)
		{
			printf("%c",'0'+(img_histogram[i]%100)/10);
		}
		printf("%c",'0'+img_histogram[i]%10);
		if(i!=255)
		{
			printf(",");
		}
	}
	printf(";\r\n");
	fclose(stdout);
}
