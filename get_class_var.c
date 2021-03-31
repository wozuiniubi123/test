#include "get_class_var.h"
#include "histogram.h" 
#include "math.h"

#define Step   4							//仅限1,2,4,8等2的次方 
#define Step_yu    (256-Step)				//4步   等差  灰度级

uint64 img_class_var[256];
uint64 histogram_cdf[256];					//最大IMG_H*IMG_W 
uint64 histogram_u_cdf[256];				//最大IMG_H*IMG_W*255

void get_class_var()
{
	int i;
	uint16 n0,n1;
	get_histogram();
	histogram_cdf[0]=img_histogram[0];
	histogram_u_cdf[0]=0;
	for(i=1;i<256;i++)
	{
		histogram_cdf[i]=histogram_cdf[i-1]+img_histogram[i];
		histogram_u_cdf[i]=histogram_u_cdf[i-1]+img_histogram[i]*i;
	}
	for(i=0;i<256;i=i+Step)
	{
		n0=histogram_cdf[i];
		n1=histogram_cdf[255]-n0;			//histogram_cdf[255]就是取用的图像的像素点的多少 
		/*
		w0=n0/histogram_cdf[255];
		w1=n1/histogram_cdf[255];
		u0=histogram_u_cdf[i]/n0;
		u1=(histogram_u_cdf[255]-histogram_u_cdf[i])/n1;
		u=histogram_u_cdf[255]/histogram_cdf[255];
		g=w0*(u0-u)*(u0-u)+w1*(u1-u)*(u1-u);
		
		g=w0*(u-u0)*(u-u0)+w1*(u1-u)*(u1-u);
		
		g=n0/histogram_cdf[255]*(histogram_u_cdf[255]/histogram_cdf[255]-histogram_u_cdf[i]/n0)^2 
		 +n1/histogram_cdf[255]*((histogram_u_cdf[255]-histogram_u_cdf[i])/n1-histogram_u_cdf[255]/histogram_cdf[255])^2;
		 
		g=n0/histogram_cdf[255]*((histogram_u_cdf[255]*n0-histogram_u_cdf[i]*histogram_cdf[255])/(n0*histogram_cdf[255]))^2
		 +n1/histogram_cdf[255]*(((histogram_u_cdf[255]-histogram_u_cdf[i])*histogram_cdf[255]-histogram_u_cdf[255]*n1)/(n1*histogram_cdf[255]))^2;
		
		g=n0*(histogram_u_cdf[255]*n0-histogram_u_cdf[i]*histogram_cdf[255])*(histogram_u_cdf[255]*n0-histogram_u_cdf[i]*histogram_cdf[255])/
			(histogram_cdf[255]*(n0*histogram_cdf[255])*(n0*histogram_cdf[255]))
			+n1*((histogram_u_cdf[255]-histogram_u_cdf[i])*histogram_cdf[255]-histogram_u_cdf[255]*n1)*((histogram_u_cdf[255]-histogram_u_cdf[i])*histogram_cdf[255]-histogram_u_cdf[255]*n1)/
			(histogram_cdf[255]*(n1*histogram_cdf[255])*(n1*histogram_cdf[255]));
		*/
		if(n0!=0&&n1!=0)
		{
			img_class_var[i]=(histogram_u_cdf[255]*n0-histogram_u_cdf[i]*histogram_cdf[255])*(histogram_u_cdf[255]*n0-histogram_u_cdf[i]*histogram_cdf[255])/n0
			+((histogram_u_cdf[255]-histogram_u_cdf[i])*histogram_cdf[255]-histogram_u_cdf[255]*n1)*((histogram_u_cdf[255]-histogram_u_cdf[i])*histogram_cdf[255]-histogram_u_cdf[255]*n1)/n1;
		}
		else
		{
			img_class_var[i]=0;
		}
	}
}

void prf_class_var()
{
	freopen("output_class_var.txt","a",stdout);
	get_class_var();
	int i,j,z;
	for(i=0;i<256;i++)
	{
		img_class_var[i]=img_class_var[i]/(histogram_u_cdf[255]);
		for(j=10;j>=1;j--)
		{
			z=pow(10, j);
			if(img_class_var[i]>=z)
			{
				printf("%c",'0'+(img_class_var[i]%(z*10))/z);
			}
		}
		printf("%c",'0'+img_class_var[i]%10);
		if(i!=255)
		{
			printf(",");
		}
	}
	printf(";\r\n");
	fclose(stdout);
}
