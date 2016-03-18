/***********************************************************************
 * 文件描述         ：
 * 头文件调用       ：
 * 变量描述         ：
 * 函数功能描述     ：
 * 作者             ：e芯7号
 * 完成时间         ： 
 * 修改时间及内容   ：
************************************************************************/
#ifndef _KALMAN_H_
#define _KALMAN_H_
#include "include.h"
void OutPut_Data(void);
void kalman_update(void);
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);

#endif