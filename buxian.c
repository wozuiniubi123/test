#include "include.h"

uint8 picture_start=59;
int8 road_end=0;
int8 hang_search=0,lie_search=0;							//道路结束的行 
int8 left_edge[IMAGE_H],right_edge[IMAGE_H],midline[IMAGE_H+1],mid_chazhi[20];	//左右边沿和中线 
 
//////////////////////////////////////
int8 L_edge_find_flag=0;
int8 R_edge_find_flag=0;
int8 L_first_edge[2];
int8 R_first_edge[2];
int8 cross=0; //判断是否为十字路口 
int8 left_turn=0;//左转弯 
int8 right_turn=0;//右转弯 
/////////////////////////////////////

int i,j;
void change_midline()//显示中线 
{
	//int i;
	for(i=picture_start;i>road_end;i--)
	{
		if(midline[i]>=0&&midline[i]<=IMG_W-1&&img[i][midline[i]]==white)
			img[i][midline[i]]=black;
	}
}
void initi()//初始化函数 
{
	memset(left_edge,-1,sizeof(left_edge));
    memset(right_edge,-1,sizeof(right_edge));   
    memset(midline,-1,sizeof(midline));	
    memset(mid_chazhi,0,sizeof(mid_chazhi));
    L_first_edge[0]=-1;
    L_first_edge[1]=-1;
    R_first_edge[0]=-1;
    R_first_edge[1]=-1;
    cross=0;
    left_turn=0;
	right_turn=0;
}

	
/*注释函数不用看，是其他思路 //////////////////////////////// 
void L_edge_find()
{
	if(img[i-1][L_first_edge[1]]==black&&img[i-2][L_first_edge[1]]==black)//向右找 
	{
		for(j=L_first_edge[1];j<=L_first_edge[1]+20;j++) 
		{
			if(j>=77)
			{
				L_edge_find_flag=0;
				break;	
			}
			if(img[i-1][j]==black&&img[i-1][j+1]==white&&img[i-1][j+2]==white)//&&img[i-1][j+3]==white
			{
				left_edge[i-1]=j;
				L_edge_find_flag=1;
				break;
			}
		}
	}
	else if(img[i-1][L_first_edge[1]]==white&&img[i-2][L_first_edge[1]]==white) //xiang 左边找 
	{
		for(j=L_first_edge[1];j>=L_first_edge[1]-10;j--)
		{
			if(j<=3)
			{
				L_edge_find_flag = 0;
				break;
			}
			if(img[i-1][j]==white&&img[i-1][j-1]==black&&img[i-1][j-2]==black)//&&img[i-1][j-3]==black
			{
				left_edge[i-1]=j-1;
				L_edge_find_flag=1;
				break;
			}
		}
		
	} 
	
	 
}
void R_edge_find()
{
	if(img[i-1][R_first_edge[1]]==black&&img[i-2][R_first_edge[1]]==black)//向左边找 
	{
		for(j=R_first_edge[1];j>=R_first_edge[1]-20;j--)
		{
			if(j<=3)
			{
				R_edge_find_flag = 0;
				break;
			}
			if(img[i-1][j]==black&&img[i-1][j-1]==white&&img[i-1][j-2]==white&&img[i-1][j-3]==white)
			{
				right_edge[i-1]=j;
				R_edge_find_flag=1;
				break;
			}
		}
	}
	else if(img[i-1][R_first_edge[1]]==white&&img[i-2][R_first_edge[1]]==white) 
	{
		for(j=R_first_edge[1];j<=R_first_edge[1]+10;j++){
			if(j>=77)
			{
				R_edge_find_flag = 0;
				break;
			}
			if(img[i-1][j]==white&&img[i-1][j-1]==black&&img[i-1][j-2]==black&&img[i-1][j-3]==black)
			{
				right_edge[i-1]=j-1;
				R_edge_find_flag=1;
				break;
			}
		}
		
	}
}


void middline_extract()
{
	//
	
	//
	if(L_first_edge[0]>R_first_edge[0]) //左边起始点低时 
	{
		midline[L_first_edge[0]]=L_first_edge[1]+35;
		for(i=L_first_edge[0]-1;i>=10;i--)
		{
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //此行的左右边缘都存在
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//左边存在 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//右存在 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//都不存在 
			{
				if(i>=25)
				{
					midline[i]=40;
				}
				 
			}

		} 
	}
	else if(L_first_edge[0]<R_first_edge[0])
	{
		midline[R_first_edge[0]]=R_first_edge[1]-35;
		for(i=R_first_edge[0]-1;i>=10;i--)
		{
			if (left_edge[i]!=-1&&right_edge[i]!=-1)   //此行的左右边缘都存在
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//左边存在 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//右存在 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//都不存在 
			{
				if(i>=25)
				{
					midline[i]=40;
				}
				 
			}

		} 
	}
	else
	{
		
		for(i=L_first_edge[0]-1;i>=10;i--)
		{
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //此行的左右边缘都存在
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//左边存在 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//右存在 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//都不存在 
			{
				if(i>=25)
				{
					midline[i]=40;
				}
				 
			}

		} 
	}
	
	for(i=IMG_H-1;i>=10;i--)
	{
		if(img[i][midline[i]]==black)
		{
			img[i][midline[i]]==white;
		}
		else 
		{
			img[i][midline[i]]=black;
		}
			
	}
	
}
*/

void edge_extract()//边缘查找里边含有对中线的提取
{				   
	init();
//	int8 correct1[60];//弯道矫正数组 
	/*for(j=59;j>=20;j--)
	{
		correct1[j]=j;
	}
	for(j=19;j>=0;j--)
	{
		correct1[j]=j+1;
	}*/
	
	//先看图像的两侧，判断大致的赛道类型 
	for(i=IMG_H;i>=20;i--)
	{
		//两边都白
		if(img[i][0]==white&&img[i][1]==white&&img[i][79]==white&&img[i][78]==white&&img[i-1][0]==white&&img[i-1][1]==white&&img[i-1][79]==white&&img[i-1][78]==white)  
		{
			int ii=i;
			while(!(img[ii][0]==black&&img[ii][1]==black&&img[ii][79]==black&&img[ii][78]==black))
			{
				ii--;
			}//ii从while循环出来后，存储的是刚有边沿时的行下标
			ii-=2;//容错
			
			//用十字上边两侧边沿的中值向下补线 
			int mid_key=0;
			int left_key=-1;
			int right_key=-1;
			for(j=40;j>=3;j--)//搜边 
			{
				if(img[ii][j]==white&&img[ii][j-1]==black&&img[ii][j-2]==black) //&&img[i][j-3]==black
				{
					left_key=j-1;
					break; 
				}
				
			} 
			for(j=40;j<=77;j++)//搜边 
			{
				if(img[ii][j]==white&&img[ii][j+1]==black&&img[ii][j+2]==black) //&&img[i][j+3]==black
				{
					right_key=j+1;
					break; 
				}
			
			}
			mid_key=(left_key+right_key)/2;
			midline[i]=mid_key;
			
		}
		
		//左白右面黑 
		else if(img[i][0]==white&&img[i][1]==white&&img[i][79]==black&&img[i][78]==black&&img[i-1][0]==white&&img[i-1][1]==white&&img[i-1][79]==black&&img[i-1][78]==black) 
		{
			
			for(j=79;j>0;j--)//搜边 
			{
				if(img[i][j]==black&&img[i][j-1]==white&&img[i][j-2]==white)
				{
					right_edge[i]=j;
					break;
				}
			}
			midline[i]=right_edge[i]-35;//35
			
			
			//作用类似于矫正数组 
			if(abs(midline[i]-midline[i+1])>=10&&abs(midline[i]-midline[i+2])>=10&&abs(midline[i]-midline[i+3])>=10&&abs(midline[i]-midline[i+4])>=10)
			{
				midline[i]+=10;
			}
			if(midline[i]<12) 
			{
				midline[i]=-1;
			} 	
				
			
			
		
		
			//以下代码中的值需要根据输出图像修改 //////////////////////
			/*
			if(right_edge[i]>=60)
			{
				midline[i]=right_edge[i]/2;
			}
			else if(right_edge[i]>=50&&right_edge[i]<60)
			{
				int t=0;
				t=midline[i+1]-1;
				if(t>=0) 
				{
					midline[i]=t;
				}
				else
				{
					
				}
				
			}
			else if(right_edge[i]<50)
			{
				//midline[i]=midline[50];
			}
			*/
			////////////////////////////////////// /////////////////////////
		}
		
		//左黑右白，与上边左白右黑对称处理即可 
		else if(img[i][0]==black&&img[i][1]==black&&img[i][79]==white&&img[i][78]==white&&img[i-1][0]==black&&img[i-1][1]==black&&img[i-1][79]==white&&img[i-1][78]==white)  
		{
			for(j=0;j<79;j++)
			{
				if(img[i][j]==black&&img[i][j+1]==white&&img[i][j+2]==white)
				{
					left_edge[i]=j;
					break;
				}
			}
			midline[i]=left_edge[i]+40;
			if(abs(midline[i]-midline[i+1])>=10&&abs(midline[i]-midline[i+2])>=10&&abs(midline[i]-midline[i+3])>=10&&abs(midline[i]-midline[i+4])>=10)
			{
				midline[i]-=15;
			}
			
			if(midline[i]>69) 
			{
				midline[i]=-1;
			}
		}
		else if(img[i][0]==black&&img[i][1]==black&&img[i][79]==black&&img[i][78]==black) //两边黑 
		{
			//搜索左边线 
			for(j=40;j>=3;j--)
			{
				if(img[i][j]==white&&img[i][j-1]==black&&img[i][j-2]==black) //&&img[i][j-3]==black
				{
					left_edge[i]=j-1;
					//L_first_edge[0]=i;
    				//L_first_edge[1]=j-1;
					L_edge_find_flag=1;
					break; 
				}
				if(L_edge_find_flag==0)
				{
					left_edge[i]=-1;
				}
			}
			//搜索右边线 
			for(j=40;j<=77;j++)
			{
				if(img[i][j]==white&&img[i][j+1]==black&&img[i][j+2]==black) //&&img[i][j+3]==black
				{
					right_edge[i]=j+1;
					//R_first_edge[0]=i;
    				//R_first_edge[1]=j+1;
					R_edge_find_flag=1;
					break; 
				}
				if(R_edge_find_flag==0)
				{
					right_edge[i]=-1;
				}
			}
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //此行的左右边缘都存在
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			
		} 	
	}
	
	//边缘显示 ,需要优化。。。 
	/*for (i =59; i >= 10; i--)
	{
		if (left_edge[i] != -1 )
		{
			img[i][left_edge[i] + 3] = black;
		}
		if (right_edge[i] != -1)
		{
			img[i][right_edge[i] - 3] = black;
		}
	}*/
} 


void mid_strengthen()//为了让中线看起来更顺滑一些以及处理个别特殊情况的误判
{
	//处理落单的中点 
	for(i=45;i>=22;i--)
	{
		
		if(midline[i+1]==midline[i+2]&&midline[i+2]==midline[i+3]&&midline[i+4]==midline[i+3])
		{
			if(abs(midline[i]-midline[i+1])>=4) 
			{
				midline[i]=midline[i+1];
			}
		}
	}	
	
	//针对倾斜的十字路口 
	int l_white=0;
	int r_white=0; 
	for(i=59;i>=20;i--)
	{
		if((img[i][0]==white&&img[i][1]==white&&img[i][2]==white)||(img[i][0]==black&&img[i][1]==white&&img[i][2]==white))
		{
			l_white++;
		}
		if((img[i][79]==white&&img[i][78]==white)||(img[i][79]==black&&img[i][78]==white&&img[i][77]==white))
		{
			r_white++;
		}
	}	
	
	if(l_white>=5&&r_white>=5)
	{
		//计算入十字之前直道的变化均值 
		int sum=0;
		for(i=57;i>=40;i--)
		{
			mid_chazhi[i]=abs(midline[i]-midline[i-1]);
			sum+=mid_chazhi[i];
		}
		int ave_chazhi=sum/18;
		
		//修正 
		for(i=40;i>=20;i--)
		{
			
			//如果误差较大 
			if(abs(midline[i]-midline[i+1])>=ave_chazhi*4&&abs(midline[i]-midline[i+2])>=ave_chazhi*4&&abs(midline[i]-midline[i+3])>=ave_chazhi*4)
			{
				//判断正常路径倾向
				if(midline[i+1]>=midline[i+2]&&midline[i+2]>=midline[i+3]) //右倾 
				{
					midline[i]=midline[i+1]+ave_chazhi;
				}
				else if(midline[i+1]<=midline[i+2]&&midline[i+2]<=midline[i+3]) //左倾 
				{
					midline[i]=midline[i+1]-ave_chazhi;
				}
				else                                          //无明显倾向 
				{
					midline[i]=midline[i+1];
				}
			}
		}
	
	}
/*	int black_flag=1;
	int white_flag=1;
	for(i=59;i>=10;i--)
	{
		if(img[i][0]==white)
		{
			black_flag=0;
		}
	}
	for(i=30;i>=10;i--)
	{
		if(img[i][79]==black)
		{
			white_flag=0;
		}
	}
	if(black_flag==1&&white_flag==1)
	{
		for(i=55;i>=44;i--)
		{
			if(midline[i]-midline[i+1]>=4&&midline[i]-midline[i+2]>=4&&midline[i]-midline[i+3]>=4)
			{
				midline[i]=midline[i+1];
			}
		} 
	}*/
	
} 


