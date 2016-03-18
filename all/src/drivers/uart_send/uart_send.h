/***********************************************************************
 * 文件描述         ：串口发送处理函数的头文件
 * 头文件调用       ：
 * 变量描述         ：
 * 函数功能描述     ：
 * 作者             ：e芯7号
 * 完成时间         ： 
 * 修改时间及内容   ：
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