/***********************************************************************
 * �ļ�����         �����ڷ��ʹ�������ͷ�ļ�
 * ͷ�ļ�����       ��
 * ��������         ��
 * ������������     ��
 * ����             ��eо7��
 * ���ʱ��         �� 
 * �޸�ʱ�估����   ��
************************************************************************/
#ifndef _UART_SEND_H_
#define _UART_SEND_H_
#include "include.h"
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void OutPut_Data();
void TSL1401_output_data();
void  uart_android();
void OutPut_speed_Data() ;
void TSL1401_output_Pixel();
void TSL1401_output_Pixel_now();
#endif