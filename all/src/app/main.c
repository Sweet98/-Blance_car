/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��main.c
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "include.h"
#include "motor.h"
#include "balance.h"
#include "kalman.h"
#include "ccd.h"
#include "uart_send.h"
#include "direction.h"
#include "coder.h"
#include "speed.h"
#include "lcd.h"
#include  "infrared.h"

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*                               Ұ�𹤳�ģ��
*  ʵ��˵����Ұ�𹤳�ģ��
*
*  ʵ�������
*
*  ʵ��Ч����
*
*  ʵ��Ŀ�ģ�
*
*  �޸�ʱ�䣺
*
*  ��    ע��
*************************************************************************/

#define Cycletime 1    //�������ڣ���λMS
#define Baud 115200    //���ڷ��Ͳ�����
static int time=0;      //ʱ��
static int time1=0;
static int time2=0;
static int time3=0;
int i=0;
int j=0;
float gyro_zero=0;
float speed_goal=0.0;
float speed=120.0;
extern float speed_coder_r;
extern float speed_coder_l;
float speed_ave=0;
float direct_gyro_zero=0.0;
int flag=0;
int xianshi=0;
char trans_flag=0;
char display_flag=1;
extern unsigned int black_pixel_num;
extern int threshold_beitai;
extern int threshold;
extern float balance_out_l;
extern float balance_out_r;
extern float direction_out_l;
extern float direction_out_r;
extern float speed_out_data;
extern float ready;
int infrare_en=0;
void main()
{
    dip_key_init();
    dip_key_get();
   
    change_parameter();    //����
    //gpio_init (PORTA,15, GPO,0); 
    gpio_init (PORTA,4, GPO,1);
    gpio_init (PORTA,5, GPO,0);
    gpio_init (PORTD,0, GPO,1);
    gpio_init (PORTB,20, GPO,1);    //�Թ�
    gpio_init (PORTB,22, GPO,1);    //�Թ�
    adc_init(ADC0, SE13);    //Acc_z   b3
    adc_init(ADC0, SE12);    //Gyro    b2
    adc_init(ADC1, SE15);    //b11      direct_gyro
    adc_init(ADC0, SE8 );    //power ad
    adc_init(ADC0, SE9 );    //�ҶԹ�ad�ɼ�
    adc_init(ADC0, SE18 );    //��Թ�ad�ɼ�
    uart_init (UART2,Baud);   //��ʼ������
    motor_init();
    TSL_1401_init();
    infrared_init();                                               //��ʼ������
    delay();
    delay();
    //xianshi=1;           //1:ƽ��   2���ٶ�   3������     4��ʵʱ��ʾ    5����̬��ʾ��ֵ��  6����̬��ʾ����ֵ
    for(i=0;i<100;i++)           //�ɼ���ƫֵ
    {
        gyro_zero=gyro_zero+ ad_once(ADC0, SE12, ADC_12bit);
        direct_gyro_zero=direct_gyro_zero+ad_once(ADC1, SE15, ADC_12bit);
        delayms(11);
    }    
    gyro_zero=gyro_zero/100.0+0.0;
    direct_gyro_zero=direct_gyro_zero/100.0-0.0;
    for(i=0;i<30;i++)
    {
        TSL1401_Pixel_updata();
    }
    gpio_set (PORTA, 5, 1);
    speed_coder_init();
    pit_init_ms(PIT0,Cycletime);
    while(1)    //��ѭ��
    {
      while(trans_flag==0)      //������û��������ѭ��
      {
      }
      if(xianshi==4)
      {
          trans_flag=0;
          TSL1401_output_Pixel_now();
          display_flag=1;
      } 
    }
     
}
int   infrare_flag;///////////////////////
int   run_stop = 0;    //////////////////////
//    0 waite
//    1 run
//    2 stop_waite
//    3 stop

int   light_black  = 0;////////////////
//  0 black
//  1 light
//
int  infrare_num;///////////////////////////
void PIT0_IRQHandler(void)  
{
      static int infrare_i =0;          ///////////////////
      static int  infrare_times = 0; //////////////////////////
    
      
      time++;
      if(time==1)
        {
           /*if(time3==0)
            {
             gpio_set (PORTA, 5, 1);
             time3=1;
            }
             else
             {
             gpio_set (PORTA, 5, 0) ;
             time3=0;
             }*/
         kalman_update();
         speed_coder_get();
         speed_ave=(speed_coder_r+speed_coder_l)/2+speed_ave;
         gpio_set  (PORTB, 19, 0);        ////////////////
        }
      if(time==2)
        {
             gpio_set  (PORTB, 19, 1);                       /////////////////////
        
             TSL1401_Pixel_updata();
             if(flag<5)   //ȡ������ֵ
             {
             threshold_get();
             threshold_beitai=threshold;
             flag++;
             }
             /*if((threshold_flag==1)&&(flag>=5))
             {
               threshold_get();
               threshold_flag=0;
               flag=5; 
             }*/
             
             TSL1401_Pixel_divide();
             
           
             
             
             
         
        }
      if(time==3)
      {
            if(xianshi!=7)
            {
             direction_out();           //2ms 
            }
             if(xianshi==5)
             {
                 TSL1401_output_data();   //5m
             }
             if(xianshi==6)
             {
                 TSL1401_output_Pixel();
             }
             //uart_android();
          
      }
      if(time==6)
        {
         //speed_angle_out(); 
       
               
         if(time3==10)
         {
           
              if(xianshi!=7)
              {
              speed_out();
              }
           
         time3=0;
         speed_ave=0;
         } 
         balance_out();
                
        }
      if(time==8) 
        {
          
          if((run_stop==3)&&(infrare_en==0))
          {
            balance_out_l=0;direction_out_l=0;speed_out_data=0;
            balance_out_r=0;direction_out_r=0;
            
          }
        motor_control();
        }
      if(time==9)
      {
          
          if((infrare_en==0)||(infrare_en==6))
          {
              if(run_stop==1)
              {
                ready=0;
                speed_goal=speed;  //120
                //time2=300;
              }
          }
          else
          {
              if(time2>(infrare_en*100))
              {
                 ready=0;
                 speed_goal=speed;  //120
                 time2=infrare_en*100;
              }
          }
          
      }
      if(time>=10)
        {
         time=0;
         time1++;
         time2++;
         time3++;
         infrare_times ++;                 ////////////////
         //time4++; 
         if((infrare_num > 7 )&&(infrare_num < 12))
         {     
             infrare_flag = 1;
         }
         else
             {
                 infrare_flag = 0;
             }  
             infrare_num = 0;
                                         ///////////////////
          
                 if (infrare_flag == 1)   /////////  
                 {
                     infrare_flag = 0; 
                     infrare_i ++;
                 }  
                 if(infrare_times == 10)
                 {
                     infrare_times = 0;             
                     if(infrare_i > 8)
                     {
                          //gpio_set(PORTA,  4, 1); ///////////////////////
                          infrare_i = 0;
 
                         if(run_stop == 1)
                          {
                              run_stop = 2;     // stop_waite
                              gpio_set(PORTA,  4, 0); /////////////////////// 
                      
                             
                            
                          }                           
                     }
                     else
                     {    
                          //gpio_set(PORTA,  4, 0); ///////////////////////
                       
                         if(run_stop == 0)
                        {
                              run_stop  = 1;     //run
                              gpio_set(PORTA,  4, 1); /////////////////////// 
                              
                              
                         }  
                         if(run_stop == 2)
                         {
                               run_stop = 3;    //stop
                               gpio_set(PORTA,  4, 1); /////////////////////// 
                         
                              
                         }  
                     }
                      infrare_i = 0;
            
                }   
         
        }
    
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}
