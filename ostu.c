#include "ostu.h"
#include "cut.h"

#define Step   1							//仅限1,2,4,8等2的次方 
#define Step_yu    (256-Step)				//4步   等差  灰度级


uint8 get_ostu_thres()
{
	uint16 i, j;
	
	//所使用的图像的范围 
	uint8 height_start=10;
	uint8 height = IMG_H;
	uint8 width_start=0;
	uint8 width = IMG_W;
	
	uint16 histogram[256]={0};						//直方图 
	uint16 histogram_cdf[256]={0};					//直方图的累计分布图最大IMG_H*IMG_W 
	uint32 histogram_u_cdf[256]={0};				//最大IMG_H*IMG_W*255
	
    uint8 find_gray_start=0;						//找到灰度值起始点的标志位 
	uint8 gray_start;								//图像的起始灰度
	uint8 gray_end;									//图像的终止灰度 
	uint8 gray_ave;									//图像的平均灰度值
	 
	uint8 bisection_left,bisection_right;			//二分法的起始左和右 
    uint8 bisection_max_dis=Step*2;					//二分法左右两端的最大差距
    uint16 n0,n1;									//阈值左右两边的像素点数 
    uint64 class_var_left,class_var_right;			//左右两边的类间方差 
	uint64 class_var_i;								//i处的类间方差
	 
	//做出灰度直方图
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			histogram[img[i][j]&Step_yu]++;			//Step_yu等于252时，相当于与上0xFC,忽略末尾四位 
		}
	}
	
	histogram_cdf[0]=histogram[0];
	histogram_u_cdf[0]=0;
    for(i=Step;i<256;i=i+Step)
    {
    	histogram_cdf[i]=histogram_cdf[i-Step]+histogram[i];
		histogram_u_cdf[i]=histogram_u_cdf[i-Step]+histogram[i]*i;
		if(!find_gray_start&&histogram[i])
		{
			gray_start=i;
		}
		else if(find_gray_start&&!histogram[i])
		{
			gray_end=i-Step;
		}
	}
	gray_ave=(uint8)(histogram_u_cdf[Step_yu]/(uint32)histogram_cdf[Step_yu])&Step_yu;
	
	bisection_left=(uint8)(histogram_u_cdf[gray_ave]/(uint32)histogram_cdf[gray_ave])&Step_yu;
	bisection_right=(uint8)((histogram_u_cdf[Step_yu]-histogram_u_cdf[gray_ave])/
					(uint32)(histogram_cdf[Step_yu]-histogram_cdf[gray_ave]))&Step_yu;
	
	n0=histogram_cdf[bisection_left];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]就是取用的图像的像素点的多少 
	class_var_left=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	n0=histogram_cdf[bisection_right];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]就是取用的图像的像素点的多少 
	class_var_right=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	//二分法 
	for(i=((bisection_left+bisection_right)>>1)&Step_yu;(bisection_right-bisection_left)>bisection_max_dis;i=((bisection_left+bisection_right)>>1)&Step_yu)
	{
		n0=histogram_cdf[i];
		n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]就是取用的图像的像素点的多少
		
		class_var_i=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
		
		if(class_var_i>=class_var_left&&class_var_right>=class_var_left)		//替换掉左边的值 
		{
			bisection_left=i;
			class_var_left=class_var_i;
		}
        else if(class_var_i>=class_var_right&&class_var_left>=class_var_right)	//替换掉右边的值 
        {
			bisection_right=i;
			class_var_right=class_var_i;
        }
        ////排除干扰的情况下出现的中间的值最小的时候  就是死循环了
        else if(class_var_i<=class_var_left&&class_var_i<=class_var_right)
        {
          	//左右两边取最大的
            if(class_var_left>=class_var_right)
			{
				bisection_right=i;               //替换右边
				class_var_right=class_var_i;
			}
            else
			{
				bisection_left=i;                //替换左边
				class_var_left=class_var_i;
			}
        }
	}
	 
	return i;
}

/*
uint8 get_ostu_thres()
{
	uint8 gray_start=0;			//图像的起始灰度 
	uint8 gray_end=255;			//图像的终止灰度 
	
	//处理图像 只需要处理20-60行
	uint8 height_start=10;
	uint8 width_start=0;
	
	uint8  width = IMG_W;  
	uint8  height = IMG_H;
	
	uint16 i, j, pixelSum = (width-width_start) * (height-height_start);  
	
	uint16  pixelCount[256]={0};  
	float pixelPro[256]={0};
	
	uint8 i_left,i_right;
    
	//做出灰度直方图 
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			pixelCount[img[i][j]&Step_yu]++;			//Step_yu等于252时，相当于与上0xFC,忽略末尾四位 
		}
	}
	
	//找到灰度值的起始点和终止点
    uint8 start_ok=0;
    for(i=0;i<256;i=i+Step)       
    {
		if(pixelCount[i])			//可以记载最后一个有用值
		{
			gray_end=i;
			start_ok=1;				//以后不再记录start值
		}
		else if(!start_ok)
		{
			gray_start=i;
		} 
	}
    
    float gray_mean=0;				//图像的灰度的均值 
    
    //计算每个像素在整幅图像中的比例，及整体平均灰度值 
    for(i = 0; i < 256; i=i+Step)  
    {
        pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);  
        gray_mean=i*pixelPro[i]+gray_mean;    
    }
    
    uint8 s_gray_mean=(uint8)gray_mean;		//图像的灰度的均值
   
	//算一下图像的灰度的平均值处的类间方差
    uint32 q_gray_mean=0,h_gray_mean=0;
    uint32 q_sum=0,h_sum=0;
    for(i = gray_start; i < gray_end; i=i+Step)  
    {
		if(i<s_gray_mean)
		{    
			q_sum=q_sum+pixelCount[i];
			q_gray_mean=i*pixelCount[i]+q_gray_mean;
		}
		else
		{
			h_sum=h_sum+pixelCount[i];
			h_gray_mean=i*pixelCount[i]+h_gray_mean; 
		}
    }
	q_gray_mean=q_gray_mean/q_sum;
	h_gray_mean=h_gray_mean/h_sum;  

    //经典ostu算法,得到前景和背景的分割  
    //遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值  
    float q_w0, q_w1, q_u0tmp, q_u1tmp, q_u0, q_u1, q_u;  
    float h_w0, h_w1, h_u0tmp, h_u1tmp, h_u0, h_u1, h_u; 
    float deltaTmp_left,deltaTmp_right;
	q_w0 = q_w1 = q_u0tmp = q_u1tmp = q_u0 = q_u1 = q_u  = 0;  
	h_w0 = h_w1 = h_u0tmp = h_u1tmp = h_u0 = h_u1 = h_u  = 0;

	for(j = gray_start; j < gray_end; j=j+Step)  
	{  
		if(j <= h_gray_mean) //背景部分  
		{  
			//以i为阈值分类，第一类总的概率  
			h_w0 += pixelPro[j];        
			h_u0tmp += j * pixelPro[j];  
		}  
		else       //前景部分  
		{  
			//以i为阈值分类，第二类总的概率  
			h_w1 += pixelPro[j];        
			h_u1tmp += j * pixelPro[j];  
		}  
		if(j <= q_gray_mean) //背景部分  
		{  
			//以i为阈值分类，第一类总的概率  
			q_w0 += pixelPro[j];        
			q_u0tmp += j * pixelPro[j];  
		}  
		else       //前景部分  
		{  
			//以i为阈值分类，第二类总的概率  
			q_w1 += pixelPro[j];        
			q_u1tmp += j * pixelPro[j];  
		}
	}  
  
	q_u0 = q_u0tmp / q_w0;        //第一类的平均灰度  
	q_u1 = q_u1tmp / q_w1;        //第二类的平均灰度  
	q_u =  q_u0tmp + q_u1tmp;      //整幅图像的平均灰度 
	
	h_u0 = h_u0tmp / h_w0;        //第一类的平均灰度  
	h_u1 = h_u1tmp / h_w1;        //第二类的平均灰度  
	h_u = h_u0tmp + h_u1tmp;      //整幅图像的平均灰度 
	
	//计算类间方差 
	deltaTmp_left = q_w0 * (q_u0 - q_u)*(q_u0 - q_u) + q_w1 * (q_u1 - q_u)*(q_u1 - q_u);  
	deltaTmp_right= h_w0 * (h_u0 - h_u)*(h_u0 - h_u) + h_w1 * (h_u1 - h_u)*(h_u1 - h_u);  
    
        
    ///进行优化    使用二分法
	float w0, w1, u0tmp, u1tmp, u0,  u1, u ,deltaTmp; 
	i_left=q_gray_mean;
	i_right=h_gray_mean;
	uint8 i_chaju=8;									//除以2
	
    for(i = (i_left+i_right)>>1;(i_right-i_left)>i_chaju ; i=(i_left+i_right)>>1)  
    {  
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for(j = gray_start; j < gray_end; j=j+Step)
        {
			if(j <= i) //背景部分  
			{                                                                            
				//以i为阈值分类，第一类总的概率  
				w0 += pixelPro[j];        
				u0tmp += j * pixelPro[j];  
			}  
			else       //前景部分  
			{  
				//以i为阈值分类，第二类总的概率  
				w1 += pixelPro[j];        
				u1tmp += j * pixelPro[j];  
			}  
        }  

        u0 = u0tmp / w0;        //第一类的平均灰度  
        u1 = u1tmp / w1;        //第二类的平均灰度  
        u = u0tmp + u1tmp;      //整幅图像的平均灰度  
        //计算类间方差  
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);  
        //找出最大类间方差以及对应的阈值 
        if(deltaTmp>=deltaTmp_left&&deltaTmp_right>=deltaTmp_left)
		{
			//换掉左 边
			i_left=i;
			deltaTmp_left=deltaTmp;
		}
        else if(deltaTmp>=deltaTmp_right&&deltaTmp_left>=deltaTmp_right)
        {
			//换掉 右 边
			i_right=i;
			deltaTmp_right=deltaTmp;
        }
        ////排除干扰的情况下出现的   中间的值最小的时候  就是死循环了
        else if(deltaTmp<=deltaTmp_left&&deltaTmp<=deltaTmp_right)
        {
          // 左右两边   取最大的
            if(deltaTmp_left>=deltaTmp_right)
			{
				i_right=i;               //替换右边
				deltaTmp_right=deltaTmp;
			}
            else
			{
				i_left=i;                //替换左边
				deltaTmp_left=deltaTmp;
			}
        }
    }
	//返回最佳阈值;     //i即是最后灰度值
	return i;
}
*/
uint8 binary_img()
{
	uint8 ostu_thres=get_ostu_thres();
	uint8 i,j;
	for(i=0;i<IMG_H;i++)
	{
		for(j=0;j<IMG_W;j++)
		{
			if(img[i][j]>ostu_thres)
				img[i][j]=White;
			else
				img[i][j]=Black;
		}
	}
}
