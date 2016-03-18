
#include "motor.h"
#include "common.h"


float balance_out_l=0.0;
float balance_out_r=0.0;
float direction_out_l=0.0;
float direction_out_r=0.0;
float speed_out_data=0.0;
unsigned int a=20;
extern float acc_z_juedui;
void motor_init()
{

    gpio_init (PORTC,5, GPO,1);        //REN        右轮
    gpio_init (PORTC,1, GPO,0);        //PWM_L1  C3
    FTM_PWM_init(FTM0, CH1, 20000, 0); //PWM_L2  C4
    gpio_init (PORTC,6, GPO,1);        //LEN        左轮
    gpio_init (PORTC,4, GPO,0);        //PWM_R1  c1
    FTM_PWM_init(FTM0, CH2, 20000, 0); //PWM_R2  C2 
}

void motor_control()
{
    float control_l;
    float control_r;
    unsigned int motor_control_l=0;
    unsigned int motor_control_r=0;
    control_l=balance_out_l+direction_out_l+speed_out_data;     //左轮控制量叠加
    control_r=balance_out_r+direction_out_r+speed_out_data;    //右轮控制量叠加
    control_l=1.0*control_l;
  //  if((acc_z_juedui>3000.0)||(acc_z_juedui<1000.0))     //车倒保护   2430   1540
   // {
   //  control_l=0;
   //  control_r=0;     
   // }
    if(control_l>980.0)
    {
        control_l=980.0;
    }     
    if(control_r>980.0)
    {
        control_r=980.0;
    }
    if(control_l<-980.0)
    {
        control_l=-980.0;
    }     
    if(control_r<-980.0)
    {
        control_r=-980.0;
    } 
    if(control_l>=0.0)            //左轮前进                       //PWM输出
    {
    motor_control_l=(unsigned int)control_l; //38                          //强制转换为整型，下一步待用    
    gpio_set (PORTC,4,0);        //PWM_R1  c1          左轮
    FTM_PWM_Duty(FTM0, CH2, motor_control_l); //PWM_R2  C2 
    }
    if(control_r>=0.0)            //右轮前进
    {
    motor_control_r=(unsigned int)control_r; //23                         //强制转换为整型，下一步待用    
    gpio_set (PORTC,1,0);        //PWM_R1  c1
    FTM_PWM_Duty(FTM0, CH1, motor_control_r); //PWM_R2  C2 

    }    
    if(control_l<0.0)       //左轮后退
    {
    motor_control_l=(unsigned int) (1000+control_l);  //-38
    gpio_set (PORTC,4,1);        //PWM_R1  c1
    FTM_PWM_Duty(FTM0, CH2, motor_control_l); //PWM_R2  C2 

    }    
    if(control_r<0.0)        //右轮后退
    {
    motor_control_r=(unsigned int)(1000+control_r);  //-23   
    gpio_set (PORTC,1,1);        //PWM_R1  c1
    FTM_PWM_Duty(FTM0, CH1, motor_control_r); //PWM_R2  C2 

    }
    
     balance_out_l=0.0;
     balance_out_r=0.0;
     //direction_out_l=0.0;
     //direction_out_r=0.0;
     control_l=0.0;
     control_r=0.0;
     motor_control_l=0;
     motor_control_r=0;  
}

void motor_protect()                                     //车倒保护
{
    gpio_set(PORTC,1,0);
    gpio_set(PORTC,3,0);
    FTM_PWM_Duty(FTM0,CH1,0);
    FTM_PWM_Duty(FTM0,CH3,0);
}


