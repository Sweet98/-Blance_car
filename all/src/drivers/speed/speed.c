#include "speed.h"

extern float speed_coder_r;
extern float speed_coder_l;
extern float speed_goal;
extern float OutData_speed[4];
extern float acc_zero;
extern float acc_speed_zero;
extern float speed_out_data;
float speed_intergral=0.0;
float speed_p=8.0;   //12
float speed_p_flash=8.0;
float speed_d=0.0;   //0.03

extern float speed_ave;
float ready=0.9;
/*void speed_angle_out()
{
     
     float speed_dir=0.0;
     float speed_p=0.64;   //
     float speed_i=0.15;   //0.125
     speed_dir=speed_goal-((speed_coder_r+speed_coder_l)/2.0);
     speed_intergral=speed_intergral+speed_dir*0.01;
     speed_coder_r=0.0;
     speed_coder_l=0.0;
     acc_speed_zero=acc_zero+speed_dir*speed_p+speed_intergral*speed_i;
}*/
void speed_out()
{
     speed_p=speed_p_flash;
     float speed_dir=0.0;
     //float speed_p=8.0;   //12
     //float speed_i=0.05;   //0.03
    // float speed_d=0.0;
     speed_dir=(speed_ave/10.0)-speed_goal;
     speed_intergral=speed_intergral+speed_dir*ready/10.0;
     if(ready==0)
     {
         speed_intergral=0;
     }
     else
     {
         speed_p=4.0;
     }
    // if(speed_intergral>900.0)
     //{
     //    speed_intergral=900.0;
    // }
    // if(speed_intergral<-400.0)
    // {
    //     speed_intergral=-400.0;
    // }     
     speed_out_data=speed_dir*speed_p+speed_intergral;
     //if(speed_out_data>1200.0)
//speed_out_data=1200.0;
    // if(speed_out_data<-600.0)
     //  speed_out_data=-600.0;        
}