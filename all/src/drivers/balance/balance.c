#include "balance.h"
extern float gyro_rate;
extern float QingJiao;
extern float balance_out_l;
extern float balance_out_r;
extern int power;
float prev_kalman_out_data=0;
float balance_p=3.2;     //3.4         7.4v   4.9    130
float balance_d=60.0;     //75        8.3v   3.4    85
float balance_i=0.01;
float kalman_out_data_intergral = 0.0;
void balance_out()
{

  float kalman_out_data=QingJiao;  //
 /*  if(power>3400)
    {
        balance_p = 2.8;
    }
   else if(power>3300)
   {
        balance_p = 3.0;
   }
   else if(power>3200)
   {
        balance_p = 3.2;
   }
   else if(power>3100)
   {
        balance_p = 3.4;
   }*/
   if(power>3000)
   {
       //balance_p = 3.1-((power-3000.0)/600.0);//3.6;
   }
   else
   {
        //balance_p = 3.1;
   }
  /* if(kalman_out_data>470.0)      //±£ª§£¨∑¿÷π≥Ã–Ú≈‹∑…
   {
       kalman_out_data=470.0;
   }
   if(kalman_out_data<-270.0)
   {
       kalman_out_data=-270.0;
   }*/
     kalman_out_data_intergral=kalman_out_data_intergral+kalman_out_data*balance_i;
     //if(kalman_out_data_intergral>=800)
     //{
     //  kalman_out_data_intergral=800;
     //}
     //if(kalman_out_data_intergral<-800)
     //{
     //  kalman_out_data_intergral=-800;
     //}
     balance_out_l=kalman_out_data*balance_p+gyro_rate*balance_d*0.01;//+kalman_out_data_intergral;
     balance_out_r=kalman_out_data*balance_p+gyro_rate*balance_d*0.01;//+kalman_out_data_intergral;
    //balance_out_l=-60.0;
    //balance_out_r=20.0;
}
