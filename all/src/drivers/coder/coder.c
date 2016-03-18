#include "coder.h"
#include "uart_send.h"
unsigned long int LA_num=0;
unsigned long int LB_num=0;
unsigned long int LZ_num=0;
unsigned long int RB_num=0;
unsigned long int RZ_num=0;
unsigned long int RA_num=0;
float speed_coder_r;
float speed_coder_l;
float OutData_speed[4] = { 0 };
extern int xianshi;                          //显示
void speed_coder_init()
{
    DMA_count_Init(DMA_CH1, PTE1, 0X7FFF, DMA_rising);      //LA
    //exti_init(PORTE, 1, rising_down);         //LA
    gpio_init (PORTE, 3, GPI, 0);         //LB
    //exti_init(PORTE, 5, rising_down);         //LZ
    gpio_init (PORTE, 24, GPI, 0);         //RB
    //exti_init(PORTE,25, rising_down);         //RZ
    DMA_count_Init(DMA_CH14, PTA14, 0X7FFF, DMA_rising);     //RA
    //exti_init(PORTE,26, rising_down);         //RA          
}


void speed_coder_get()
{
     LB_num=1-gpio_get(PORTE, 3);          //正反转标志
     RB_num=gpio_get(PORTE, 24);           //正反转标志
     LA_num=DMA_count_get(DMA_CH1);        
     RA_num=DMA_count_get(DMA_CH14);
     DMA_count_reset(DMA_CH1);
     DMA_count_reset(DMA_CH14);
     if(LB_num==0)
     {
        speed_coder_l=(float)LA_num;
     }
     else
     {
        speed_coder_l=0.0-(float)LA_num;
         
     }
     if(RB_num==0)
     {
        speed_coder_r=(float)RA_num;  
     }
     else
     {
        speed_coder_r=0.0-(float)RA_num; 
     }
    OutData_speed[0]=speed_coder_r;
    OutData_speed[1]=speed_coder_l;
    RA_num=0.0;
    LA_num=0.0;
    if(xianshi==2)
    {
        OutPut_speed_Data();
    } 
}
void PORTE_IRQHandler()
{
    u8  n = 0;    //引脚号

    n = 1;
    if(PORTE_ISFR & (1 << n))           //PTE1触发中断    LA左轮速度
    {
        PORTE_ISFR  |= (1 << n);        //写1清中断标志位
        // 用户任务
        
        LA_num++;                       //左侧编码器计数
    }
    
    
    

    n = 26;      
    if(PORTE_ISFR & (1 << n))           //PTE26触发中断     RA右轮速度
    {
        PORTE_ISFR  |= (1 << n);        //写1清中断标志位
        // 用户任务
        RA_num++;                       //右侧编码器计数
    }
}