#include  "common.h"
#include  "gpio.h"
#include  "FTM.h"
#include  "delay.h"
#include  "infrared.h"
#include   "exti.h"
void infrared_init()
{
    gpio_init (PORTA,  4, 1, 0);
    exti_init (PORTB, 17, rising_up);

    gpio_init (PORTB, 19, 1, 1);
    gpio_set  (PORTB, 19, 1);
    //FTM_PWM_init(FTM2, CH1,1000,900);

}


void infrared_test()
{     int i = 10000;
    int j= 10000;
    gpio_set  (PORTB, 19, 0); 
    delayms(1);
    gpio_set  (PORTB, 19, 1);
    gpio_set(PORTA,  4, 0);
    delayms(1);
 
      for(i = 0;i<10000;i++)
     {  
         for(j =0; j<10000;j++)
         {
            if(gpio_get(PORTB, 17) == 0)
            {     
                //gpio_set(PORTA,  4, 1);
                //while(1);
                gpio_turn(PORTA, 4);
                  delayms(100);
            }
         }
    }
}