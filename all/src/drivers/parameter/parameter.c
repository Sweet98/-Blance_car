#include "parameter.h"

float Acc_z_init[20]=0;
float Gyro_init[20]=0;

void parameter_init()
{
    //Acc_z_init[] = ad_once(ADC0, SE13, ADC_12bit);     //z����Сֵ107���ֵ141�м�124 3���
    int i=0;
    for(i=0;i<20;i++)
    {
      Gyro_init[i]= ad_once(ADC0, SE12, ADC_12bit);     //��������ƫֵ105
    }
      
}