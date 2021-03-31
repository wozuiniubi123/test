#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <string.h>

typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */

typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */
typedef long  long          int64;  /* 64 bits */

#define IMAGE_H 120				//��ȡͼ��ĸ߶� 
#define IMAGE_W 188             //��ȡͼ��Ŀ��

#define IMG_H 60				//Ԥ�����ͼ��ĸ߶� 
#define IMG_W 80                //Ԥ�����ͼ��Ŀ��

#define white (255)
#define gray (128)
#define black (0)
extern uint8 image[IMAGE_H][IMAGE_W]; //���յ���ͼ��
extern uint8 img[IMG_H][IMG_W];		//Ԥ������ͼ��

#endif
