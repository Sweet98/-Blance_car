/***********************************************************************
 * �ļ�����         ��ccd�ɼ����ݣ��浽�����ͬʱ��ȡ������
 * ͷ�ļ�����       ������adc.h,ͨ��ad�ɼ��õ�128�����ص㡣
 * ��������         ��ȫ�ֱ�����Pixel_get[128] �洢128�����ص�
                                Mid_line[20]   �洢����
 * ������������     ��
 * ����             ��eо7��
 * ���ʱ��         �� 
 * �޸�ʱ�估����   ��
************************************************************************/
#include "ccd.h"
int threshold=0;
int threshold_beitai=0;
unsigned char Pixel_get[128]={0};
unsigned char Pixel_get_divide[128]={0};
unsigned int black_pixel_num=0;       //�ڵ����
float percentage=2.0;
#define TSL1401_SI(X)  gpio_set(PORTB,16,X)
#define TSL1401_CLK(X)  gpio_set(PORTB,18,X)
extern int uart_outwhat;
void delay_ccd()
{
  unsigned int i;
  for(i=0;i<20;i++)
  {
    asm("nop");
  }
}

void TSL1401_Pixel_updata()
{
  unsigned char i;
  
  //��ʼSI
  TSL1401_SI(0);
  TSL1401_CLK(0);
  delay_ccd();
  TSL1401_SI(1);
  delay_ccd();
  TSL1401_CLK(1);
  delay_ccd();
  TSL1401_SI(0);
  delay_ccd();
  
  Pixel_get[0] = ad_once(ADC1,SE14 , ADC_8bit);                    //�ɼ���1����
  TSL1401_CLK(0);
  for(i=1; i<128; i++)                                             //�ɼ���2~128����
  {
    delay_ccd();
    TSL1401_CLK(1);
    delay_ccd(); 
    Pixel_get[i] = ad_once(ADC1,SE14,ADC_8bit);
    TSL1401_CLK(0);
  }
   delay_ccd();                                                    //���͵�129��clk
   TSL1401_CLK(1);
   delay_ccd(); 
   TSL1401_CLK(0);
   delay_ccd();
}
void TSL1401_Pixel_divide()
{
    int j=0;
    threshold_get();
    
    black_pixel_num=0;
    for(j=0;j<128;j++)
    {
       if(Pixel_get[j]>threshold)
       {
        Pixel_get_divide[j]=0xaa;   //170
       //black_pixel_num++;
       }
       else
       {
        Pixel_get_divide[j]=0x55;    //85
         black_pixel_num++;
       } 
       
     }
    /*if( black_pixel_num>100)
       {
           gpio_set (PORTA, 4, 0);
       }
    else
    {
    gpio_set (PORTA, 4, 1);
    }
       black_pixel_num=0;*/
  }

void threshold_get()
{
   int i=0;
   unsigned char Pixel_max=0;
   unsigned char Pixel_min=0;
   float temp=0.0;
   Pixel_max=Pixel_get[0];
   Pixel_min=Pixel_get[0];
    for(i=1;i<128;i++)
     {
        if(Pixel_max<Pixel_get[i])
       {
        Pixel_max=Pixel_get[i];
       }
     }
    for(i=1;i<128;i++)
     {
        if(Pixel_min>Pixel_get[i])
       {
        Pixel_min=Pixel_get[i];
       }
     }
    if(Pixel_max<0x0d)
    {
      if((Pixel_max-Pixel_min)>0x03)
      {
        temp=Pixel_max/1.3;
      }
      else
      {
        temp=threshold_beitai; 
      }
    }
   else
   {
     temp=Pixel_max/percentage;
   }
   threshold=(int)temp;    //3
}
void TSL_1401_init(void)
{
    gpio_init (PORTB,16,GPO,0);
    gpio_init (PORTB,18,GPO,0); 
    adc_init(ADC1,SE14);                                                       //��ʼ��ADC0, 8λ���ȣ�����
}