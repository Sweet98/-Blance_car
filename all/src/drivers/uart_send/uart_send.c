/***********************************************************************
 * 文件描述         ：串口发送处理函数
 * 头文件调用       ：
 * 变量描述         ：
 * 函数功能描述     ：
 * 作者             ：e芯7号
 * 完成时间         ： 
 * 修改时间及内容   ：
************************************************************************/
#include  "uart_send.h"
extern float OutData[4];
extern float OutData_speed[4];
extern unsigned char Pixel_get[128];
extern unsigned char Pixel_get_divide[128];
extern unsigned char Pixel_get_now[128];
extern int mid_now;
extern int mid;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)   
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}


void OutPut_Data()     //kalman滤波输出
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
   uart_putchar (UART2, databuf[i]); //uart_putchar(UART2，databuf[i]);
}
void OutPut_speed_Data()          //16位输出速度，最大值65535
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData_speed[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
   uart_putchar (UART2, databuf[i]); //uart_putchar(UART2，databuf[i]);

}
void TSL1401_output_data()     //ccd像素二值化输出
{
         int i;     
         uart_putchar (UART2,0x02);
         uart_putchar (UART2,0xFD);
         for(i=0; i<128; i=i+1)
         {          
          
           if((i>(mid-2))&&(i<(mid+1)))
           {
             Pixel_get_divide[i]=0x00;  
           }
            uart_putchar (UART2,Pixel_get_divide[i]);   
         }
         uart_putchar (UART2,0xFD);  //此字节用于线性CCD调试助手识别换行////5
         uart_putchar (UART2,0x02);   // time_delay_ms(200);  /////////////////LPLD_LPTMR_DelayMs(1000);

}
void TSL1401_output_Pixel()       //ccd原始像素输出
{
         int i;     
         uart_putchar (UART2,0x02);
         uart_putchar (UART2,0xFD);
         for(i=0; i<128; i=i+1)
         {          
          
           if((i>(mid-2))&&(i<(mid+1)))
           {
             Pixel_get[i]=0x00;  
           }
            uart_putchar (UART2,Pixel_get[i]);   
         }
         uart_putchar (UART2,0xFD);  //此字节用于线性CCD调试助手识别换行////5
         uart_putchar (UART2,0x02);   // time_delay_ms(200);  /////////////////LPLD_LPTMR_DelayMs(1000);

}
void TSL1401_output_Pixel_now()         //实时输出ccd
{
         int i;     
         uart_putchar (UART2,0x02);
         uart_putchar (UART2,0xFD);
         for(i=0; i<128; i=i+1)
         {          
          
           if((i>(mid_now-2))&&(i<(mid_now+1)))
           {
             Pixel_get_now[i]=0x00;  
           }
            uart_putchar (UART2,Pixel_get_now[i]);   
         }
         uart_putchar (UART2,0xFD);  //此字节用于线性CCD调试助手识别换行////5
         uart_putchar (UART2,0x02);   // time_delay_ms(200);  /////////////////LPLD_LPTMR_DelayMs(1000);

}
void  uart_android()        //安卓手机接收
{
  unsigned char str[256+16+4] = {0};
  unsigned char i;
  str[0] = 0xfe;//数据头
  str[1] = 0x01;
  str[2] = 0xfe;
  str[3] = 0x01;
  for(i=0; i<128; i++)//ccd1
   {          
//     if(Pixel_get[i]==0x02)            //串口发送AD值，可用于线性CCD调试助手
//     {
//      str[i+4] = 0x01;             //遇到0x02用0x01替换即可//  
//     } 
//     if((i>(mid-2))&&(i<(mid+1)))
//     {
//       str[i+4] = 0x0c;  
//     }    
     str[i+4] = Pixel_get[i];
   }
  for(i=0; i<128; i++)//CCD2
  {
    str[i+132] = 0;
  }
  for(i=0; i<16; i++)//二值化压缩
  {
    str[i+260] = 0;
  }
  for(unsigned int j = 0;j<276;j++)
    uart_putchar (UART2,str[j]);
}