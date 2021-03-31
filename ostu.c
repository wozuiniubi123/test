#include "ostu.h"
#include "cut.h"

#define Step   1							//����1,2,4,8��2�Ĵη� 
#define Step_yu    (256-Step)				//4��   �Ȳ�  �Ҷȼ�


uint8 get_ostu_thres()
{
	uint16 i, j;
	
	//��ʹ�õ�ͼ��ķ�Χ 
	uint8 height_start=10;
	uint8 height = IMG_H;
	uint8 width_start=0;
	uint8 width = IMG_W;
	
	uint16 histogram[256]={0};						//ֱ��ͼ 
	uint16 histogram_cdf[256]={0};					//ֱ��ͼ���ۼƷֲ�ͼ���IMG_H*IMG_W 
	uint32 histogram_u_cdf[256]={0};				//���IMG_H*IMG_W*255
	
    uint8 find_gray_start=0;						//�ҵ��Ҷ�ֵ��ʼ��ı�־λ 
	uint8 gray_start;								//ͼ�����ʼ�Ҷ�
	uint8 gray_end;									//ͼ�����ֹ�Ҷ� 
	uint8 gray_ave;									//ͼ���ƽ���Ҷ�ֵ
	 
	uint8 bisection_left,bisection_right;			//���ַ�����ʼ����� 
    uint8 bisection_max_dis=Step*2;					//���ַ��������˵������
    uint16 n0,n1;									//��ֵ�������ߵ����ص��� 
    uint64 class_var_left,class_var_right;			//�������ߵ���䷽�� 
	uint64 class_var_i;								//i������䷽��
	 
	//�����Ҷ�ֱ��ͼ
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			histogram[img[i][j]&Step_yu]++;			//Step_yu����252ʱ���൱������0xFC,����ĩβ��λ 
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
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ��� 
	class_var_left=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_left]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_left])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	n0=histogram_cdf[bisection_right];
	n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ��� 
	class_var_right=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[bisection_right]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[bisection_right])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
	
	//���ַ� 
	for(i=((bisection_left+bisection_right)>>1)&Step_yu;(bisection_right-bisection_left)>bisection_max_dis;i=((bisection_left+bisection_right)>>1)&Step_yu)
	{
		n0=histogram_cdf[i];
		n1=histogram_cdf[Step_yu]-n0;			//histogram_cdf[255]����ȡ�õ�ͼ������ص�Ķ���
		
		class_var_i=((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])*
					((uint64)histogram_u_cdf[Step_yu]*n0-(uint64)histogram_u_cdf[i]*histogram_cdf[Step_yu])/(uint64)n0
					+(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)*
					(((uint64)histogram_u_cdf[Step_yu]-(uint64)histogram_u_cdf[i])*(uint64)histogram_cdf[Step_yu]-(uint64)histogram_u_cdf[Step_yu]*n1)/(uint64)n1;
		
		if(class_var_i>=class_var_left&&class_var_right>=class_var_left)		//�滻����ߵ�ֵ 
		{
			bisection_left=i;
			class_var_left=class_var_i;
		}
        else if(class_var_i>=class_var_right&&class_var_left>=class_var_right)	//�滻���ұߵ�ֵ 
        {
			bisection_right=i;
			class_var_right=class_var_i;
        }
        ////�ų����ŵ�����³��ֵ��м��ֵ��С��ʱ��  ������ѭ����
        else if(class_var_i<=class_var_left&&class_var_i<=class_var_right)
        {
          	//��������ȡ����
            if(class_var_left>=class_var_right)
			{
				bisection_right=i;               //�滻�ұ�
				class_var_right=class_var_i;
			}
            else
			{
				bisection_left=i;                //�滻���
				class_var_left=class_var_i;
			}
        }
	}
	 
	return i;
}

/*
uint8 get_ostu_thres()
{
	uint8 gray_start=0;			//ͼ�����ʼ�Ҷ� 
	uint8 gray_end=255;			//ͼ�����ֹ�Ҷ� 
	
	//����ͼ�� ֻ��Ҫ����20-60��
	uint8 height_start=10;
	uint8 width_start=0;
	
	uint8  width = IMG_W;  
	uint8  height = IMG_H;
	
	uint16 i, j, pixelSum = (width-width_start) * (height-height_start);  
	
	uint16  pixelCount[256]={0};  
	float pixelPro[256]={0};
	
	uint8 i_left,i_right;
    
	//�����Ҷ�ֱ��ͼ 
	for(i=height_start;i<height;i++)
	{
		for(j = width_start;j<width;j++)
		{
			pixelCount[img[i][j]&Step_yu]++;			//Step_yu����252ʱ���൱������0xFC,����ĩβ��λ 
		}
	}
	
	//�ҵ��Ҷ�ֵ����ʼ�����ֹ��
    uint8 start_ok=0;
    for(i=0;i<256;i=i+Step)       
    {
		if(pixelCount[i])			//���Լ������һ������ֵ
		{
			gray_end=i;
			start_ok=1;				//�Ժ��ټ�¼startֵ
		}
		else if(!start_ok)
		{
			gray_start=i;
		} 
	}
    
    float gray_mean=0;				//ͼ��ĻҶȵľ�ֵ 
    
    //����ÿ������������ͼ���еı�����������ƽ���Ҷ�ֵ 
    for(i = 0; i < 256; i=i+Step)  
    {
        pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);  
        gray_mean=i*pixelPro[i]+gray_mean;    
    }
    
    uint8 s_gray_mean=(uint8)gray_mean;		//ͼ��ĻҶȵľ�ֵ
   
	//��һ��ͼ��ĻҶȵ�ƽ��ֵ������䷽��
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

    //����ostu�㷨,�õ�ǰ���ͱ����ķָ�  
    //�����Ҷȼ�[0,255],������������ĻҶ�ֵ,Ϊ�����ֵ  
    float q_w0, q_w1, q_u0tmp, q_u1tmp, q_u0, q_u1, q_u;  
    float h_w0, h_w1, h_u0tmp, h_u1tmp, h_u0, h_u1, h_u; 
    float deltaTmp_left,deltaTmp_right;
	q_w0 = q_w1 = q_u0tmp = q_u1tmp = q_u0 = q_u1 = q_u  = 0;  
	h_w0 = h_w1 = h_u0tmp = h_u1tmp = h_u0 = h_u1 = h_u  = 0;

	for(j = gray_start; j < gray_end; j=j+Step)  
	{  
		if(j <= h_gray_mean) //��������  
		{  
			//��iΪ��ֵ���࣬��һ���ܵĸ���  
			h_w0 += pixelPro[j];        
			h_u0tmp += j * pixelPro[j];  
		}  
		else       //ǰ������  
		{  
			//��iΪ��ֵ���࣬�ڶ����ܵĸ���  
			h_w1 += pixelPro[j];        
			h_u1tmp += j * pixelPro[j];  
		}  
		if(j <= q_gray_mean) //��������  
		{  
			//��iΪ��ֵ���࣬��һ���ܵĸ���  
			q_w0 += pixelPro[j];        
			q_u0tmp += j * pixelPro[j];  
		}  
		else       //ǰ������  
		{  
			//��iΪ��ֵ���࣬�ڶ����ܵĸ���  
			q_w1 += pixelPro[j];        
			q_u1tmp += j * pixelPro[j];  
		}
	}  
  
	q_u0 = q_u0tmp / q_w0;        //��һ���ƽ���Ҷ�  
	q_u1 = q_u1tmp / q_w1;        //�ڶ����ƽ���Ҷ�  
	q_u =  q_u0tmp + q_u1tmp;      //����ͼ���ƽ���Ҷ� 
	
	h_u0 = h_u0tmp / h_w0;        //��һ���ƽ���Ҷ�  
	h_u1 = h_u1tmp / h_w1;        //�ڶ����ƽ���Ҷ�  
	h_u = h_u0tmp + h_u1tmp;      //����ͼ���ƽ���Ҷ� 
	
	//������䷽�� 
	deltaTmp_left = q_w0 * (q_u0 - q_u)*(q_u0 - q_u) + q_w1 * (q_u1 - q_u)*(q_u1 - q_u);  
	deltaTmp_right= h_w0 * (h_u0 - h_u)*(h_u0 - h_u) + h_w1 * (h_u1 - h_u)*(h_u1 - h_u);  
    
        
    ///�����Ż�    ʹ�ö��ַ�
	float w0, w1, u0tmp, u1tmp, u0,  u1, u ,deltaTmp; 
	i_left=q_gray_mean;
	i_right=h_gray_mean;
	uint8 i_chaju=8;									//����2
	
    for(i = (i_left+i_right)>>1;(i_right-i_left)>i_chaju ; i=(i_left+i_right)>>1)  
    {  
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for(j = gray_start; j < gray_end; j=j+Step)
        {
			if(j <= i) //��������  
			{                                                                            
				//��iΪ��ֵ���࣬��һ���ܵĸ���  
				w0 += pixelPro[j];        
				u0tmp += j * pixelPro[j];  
			}  
			else       //ǰ������  
			{  
				//��iΪ��ֵ���࣬�ڶ����ܵĸ���  
				w1 += pixelPro[j];        
				u1tmp += j * pixelPro[j];  
			}  
        }  

        u0 = u0tmp / w0;        //��һ���ƽ���Ҷ�  
        u1 = u1tmp / w1;        //�ڶ����ƽ���Ҷ�  
        u = u0tmp + u1tmp;      //����ͼ���ƽ���Ҷ�  
        //������䷽��  
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);  
        //�ҳ������䷽���Լ���Ӧ����ֵ 
        if(deltaTmp>=deltaTmp_left&&deltaTmp_right>=deltaTmp_left)
		{
			//������ ��
			i_left=i;
			deltaTmp_left=deltaTmp;
		}
        else if(deltaTmp>=deltaTmp_right&&deltaTmp_left>=deltaTmp_right)
        {
			//���� �� ��
			i_right=i;
			deltaTmp_right=deltaTmp;
        }
        ////�ų����ŵ�����³��ֵ�   �м��ֵ��С��ʱ��  ������ѭ����
        else if(deltaTmp<=deltaTmp_left&&deltaTmp<=deltaTmp_right)
        {
          // ��������   ȡ����
            if(deltaTmp_left>=deltaTmp_right)
			{
				i_right=i;               //�滻�ұ�
				deltaTmp_right=deltaTmp;
			}
            else
			{
				i_left=i;                //�滻���
				deltaTmp_left=deltaTmp;
			}
        }
    }
	//���������ֵ;     //i�������Ҷ�ֵ
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
