/***********************************************************************
 * �ļ�����         ��
 * ͷ�ļ�����       ��
 * ��������         ��
 * ������������     ��
 * ����             ��eо7��
 * ���ʱ��         �� 
 * �޸�ʱ�估����   ��
************************************************************************/
#ifndef _KALMAN_H_
#define _KALMAN_H_
#include "include.h"
void OutPut_Data(void);
void kalman_update(void);
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);

#endif