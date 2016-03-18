/***********************************************************************
 * 文件描述         ：ccd.c的头文件
 * 头文件调用       ：
 * 变量描述         ：
 * 函数功能描述     ：
 * 作者             ：e芯7号
 * 完成时间         ： 
 * 修改时间及内容   ：
************************************************************************/
#ifndef _CCD_H_
#define _CCD_H_
#include "include.h"
void delay_ccd();
void TSL1401_Pixel_updata();
void TSL_1401_init(void);
void TSL1401_Pixel_divide();
void threshold_get();

#endif