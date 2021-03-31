#include "include.h"

uint8 picture_start=59;
int8 road_end=0;
int8 hang_search=0,lie_search=0;							//��·�������� 
int8 left_edge[IMAGE_H],right_edge[IMAGE_H],midline[IMAGE_H+1],mid_chazhi[20];	//���ұ��غ����� 
 
//////////////////////////////////////
int8 L_edge_find_flag=0;
int8 R_edge_find_flag=0;
int8 L_first_edge[2];
int8 R_first_edge[2];
int8 cross=0; //�ж��Ƿ�Ϊʮ��·�� 
int8 left_turn=0;//��ת�� 
int8 right_turn=0;//��ת�� 
/////////////////////////////////////

int i,j;
void change_midline()//��ʾ���� 
{
	//int i;
	for(i=picture_start;i>road_end;i--)
	{
		if(midline[i]>=0&&midline[i]<=IMG_W-1&&img[i][midline[i]]==white)
			img[i][midline[i]]=black;
	}
}
void initi()//��ʼ������ 
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

	
/*ע�ͺ������ÿ���������˼· //////////////////////////////// 
void L_edge_find()
{
	if(img[i-1][L_first_edge[1]]==black&&img[i-2][L_first_edge[1]]==black)//������ 
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
	else if(img[i-1][L_first_edge[1]]==white&&img[i-2][L_first_edge[1]]==white) //xiang ����� 
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
	if(img[i-1][R_first_edge[1]]==black&&img[i-2][R_first_edge[1]]==black)//������� 
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
	if(L_first_edge[0]>R_first_edge[0]) //�����ʼ���ʱ 
	{
		midline[L_first_edge[0]]=L_first_edge[1]+35;
		for(i=L_first_edge[0]-1;i>=10;i--)
		{
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //���е����ұ�Ե������
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//��ߴ��� 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//�Ҵ��� 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//�������� 
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
			if (left_edge[i]!=-1&&right_edge[i]!=-1)   //���е����ұ�Ե������
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//��ߴ��� 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//�Ҵ��� 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//�������� 
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
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //���е����ұ�Ե������
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			else if(left_edge[i]!=-1&&right_edge[i]==-1&&left_edge[i+1]!=-1)//��ߴ��� 
			{
				midline[i]=midline[i+1]+abs(left_edge[i+1]-left_edge[i]);
			}
			else if(left_edge[i]==-1&&right_edge[i]!=-1&&right_edge[i+1]!=-1)//�Ҵ��� 
			{
				midline[i]=midline[i+1]-abs(right_edge[i+1]-right_edge[i]);
			}
			else//�������� 
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

void edge_extract()//��Ե������ߺ��ж����ߵ���ȡ
{				   
	init();
//	int8 correct1[60];//����������� 
	/*for(j=59;j>=20;j--)
	{
		correct1[j]=j;
	}
	for(j=19;j>=0;j--)
	{
		correct1[j]=j+1;
	}*/
	
	//�ȿ�ͼ������࣬�жϴ��µ��������� 
	for(i=IMG_H;i>=20;i--)
	{
		//���߶���
		if(img[i][0]==white&&img[i][1]==white&&img[i][79]==white&&img[i][78]==white&&img[i-1][0]==white&&img[i-1][1]==white&&img[i-1][79]==white&&img[i-1][78]==white)  
		{
			int ii=i;
			while(!(img[ii][0]==black&&img[ii][1]==black&&img[ii][79]==black&&img[ii][78]==black))
			{
				ii--;
			}//ii��whileѭ�������󣬴洢���Ǹ��б���ʱ�����±�
			ii-=2;//�ݴ�
			
			//��ʮ���ϱ�������ص���ֵ���²��� 
			int mid_key=0;
			int left_key=-1;
			int right_key=-1;
			for(j=40;j>=3;j--)//�ѱ� 
			{
				if(img[ii][j]==white&&img[ii][j-1]==black&&img[ii][j-2]==black) //&&img[i][j-3]==black
				{
					left_key=j-1;
					break; 
				}
				
			} 
			for(j=40;j<=77;j++)//�ѱ� 
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
		
		//�������� 
		else if(img[i][0]==white&&img[i][1]==white&&img[i][79]==black&&img[i][78]==black&&img[i-1][0]==white&&img[i-1][1]==white&&img[i-1][79]==black&&img[i-1][78]==black) 
		{
			
			for(j=79;j>0;j--)//�ѱ� 
			{
				if(img[i][j]==black&&img[i][j-1]==white&&img[i][j-2]==white)
				{
					right_edge[i]=j;
					break;
				}
			}
			midline[i]=right_edge[i]-35;//35
			
			
			//���������ڽ������� 
			if(abs(midline[i]-midline[i+1])>=10&&abs(midline[i]-midline[i+2])>=10&&abs(midline[i]-midline[i+3])>=10&&abs(midline[i]-midline[i+4])>=10)
			{
				midline[i]+=10;
			}
			if(midline[i]<12) 
			{
				midline[i]=-1;
			} 	
				
			
			
		
		
			//���´����е�ֵ��Ҫ�������ͼ���޸� //////////////////////
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
		
		//����Ұף����ϱ�����ҺڶԳƴ����� 
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
		else if(img[i][0]==black&&img[i][1]==black&&img[i][79]==black&&img[i][78]==black) //���ߺ� 
		{
			//��������� 
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
			//�����ұ��� 
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
			if(left_edge[i]!=-1&&right_edge[i]!=-1)   //���е����ұ�Ե������
			{
				midline[i]=(left_edge[i]+right_edge[i])/2;
			}
			
		} 	
	}
	
	//��Ե��ʾ ,��Ҫ�Ż������� 
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


void mid_strengthen()//Ϊ�������߿�������˳��һЩ�Լ���������������������
{
	//�����䵥���е� 
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
	
	//�����б��ʮ��·�� 
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
		//������ʮ��֮ǰֱ���ı仯��ֵ 
		int sum=0;
		for(i=57;i>=40;i--)
		{
			mid_chazhi[i]=abs(midline[i]-midline[i-1]);
			sum+=mid_chazhi[i];
		}
		int ave_chazhi=sum/18;
		
		//���� 
		for(i=40;i>=20;i--)
		{
			
			//������ϴ� 
			if(abs(midline[i]-midline[i+1])>=ave_chazhi*4&&abs(midline[i]-midline[i+2])>=ave_chazhi*4&&abs(midline[i]-midline[i+3])>=ave_chazhi*4)
			{
				//�ж�����·������
				if(midline[i+1]>=midline[i+2]&&midline[i+2]>=midline[i+3]) //���� 
				{
					midline[i]=midline[i+1]+ave_chazhi;
				}
				else if(midline[i+1]<=midline[i+2]&&midline[i+2]<=midline[i+3]) //���� 
				{
					midline[i]=midline[i+1]-ave_chazhi;
				}
				else                                          //���������� 
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


