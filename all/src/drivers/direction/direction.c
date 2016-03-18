#include "direction.h"
#include "uart_send.h"
#define mid_law 64.0
extern unsigned char Pixel_get[128];         //�õ���ԭʼ����ֵ
extern unsigned char Pixel_get_divide[128];  //��ֵ�����ֵ
extern float direction_out_l;                //�����������
extern float direction_out_r;                //�����������
extern float OutData[4];                     //OutPut_Data()�õ�������
extern char trans_flag;                      //�����ݵı�־                      
extern char display_flag;                    //��ʾ���ݵı�־
extern unsigned int black_pixel_num;         //�ڵ����Ŀ
extern float speed_goal;                     //�趨�ٶ�
extern float direct_gyro_zero;               //�������������
extern int xianshi;                          //��ʾ
extern int flag;
float coder_intergral_l=0.0;                 //�����
float coder_intergral_r=0.0;                 //�����
float coder_intergral=0.0;                 //�����
char coder_intergral_flag=0;            //������־
extern float speed_coder_r;
extern float speed_coder_l;
unsigned char Mid_line[20]={0};              //�洢����
unsigned char Pixel_get_now[128]={0};        //ʵʱ��ʾ�����ص������
int mid=64;               //��ʼ����
int mid_now=64;           //ʵʱ��ʾ�õ�������
int wide=122;             //Ĭ���������
float gap=0;              //�����߾����                          
float direct_gyro=0.0;    //ת�������ǵ�ֵ
char yuansu_flag=0;       //Ԫ�ر�־
char blackline_flag=0;    //��ֱ�Ǻ���̱�־
int blackline_time=0;     //��ֱ�Ǻ��ʱ
int blackline_time1=0;     //��ֱ�Ǻ��ӳ�
float direction_p=8.0;    //8.3
float direction_d=1.7;   //0.17
float direction_p_flash=8.3;    //8.3
float direction_d_flash=1.7;   //0.17
char barrier_flag=0;
int wide_old=122;
int wide_old_old=61;
int barrier_time=0;
float speed_angle = 70.0;
char angle_flag=0;
char single_flag=0;      //���߱�־
int angle_num_l=0;
int angle_num_r=0;
int range=60;
float direction_p_s=9.0;
float direction_d_s=1.8;
int barrier_wide=40;

void direction_out()
{

    
    int left_line=0;
    int right_line=127;
    float direct_dir=0.0;
    int i=0;
    float gap=0;
    float r_duiguan=0;
    float l_duiguan=0;
    direction_p=direction_p_flash;
    direction_d=direction_d_flash;
    //�ж�Ԫ��
   r_duiguan=ad_once(ADC0, SE9, ADC_12bit) ;
   l_duiguan=ad_once(ADC0, SE18, ADC_12bit);
   if((70<r_duiguan)&&(30<l_duiguan))   
    {
      if(blackline_flag==0)
      {
        yuansu_flag=1;
        //time4=1005;
        blackline_flag=1;
        coder_intergral_flag=1;
        gpio_set (PORTD, 0, 0); 
        /*if(((speed_coder_r+speed_coder_l)/2.0)<140)
        {
            speed_angle=50;
        }
        else
        {
            speed_angle=10;
        }*/
      }
        //speed_angle=60;
      if(blackline_flag==2)
      {
          blackline_flag=3;       
      }
    }
    if(blackline_flag==3)
    {
      if((40>r_duiguan)||(15>l_duiguan))
      {
        yuansu_flag=0;
        blackline_flag=0;
        angle_flag=0;
        angle_num_l=0;
        angle_num_r=0;
        gpio_set (PORTD, 0, 1);
        gpio_set (PORTA, 5, 1);
      }
    }
  if((coder_intergral_flag==1)&&(coder_intergral<12000))
  {
      coder_intergral_l=coder_intergral_l+speed_coder_l;
      coder_intergral_r=coder_intergral_r+speed_coder_r;
      coder_intergral=(coder_intergral_l+coder_intergral_r)/2.0;
  }  
    
    
    //����Ԫ��  
    if(yuansu_flag==0)       //һ��Ԫ�س���
    {
        for(i=mid+range;i>0;i--)       //Ѱ�߷�Χ����~0       ��Ѱ����
        {
            if(i<0)
            {
                i=0;
            }
            if(i>127)
            {
                i=127;
            }
            if((Pixel_get_divide[i]-Pixel_get_divide[i-1])==0x55)
            {
                left_line=i;        //������Сֵ1  ���ֵ127
                break;
            }
    
        }
        for(i=mid-range;i<127;i++)    //Ѱ�߷�Χ����~127
        {
            if(i<0)
            {
                i=0;
            }
            if(i>127)
            {
               i=127;
            }
            if((Pixel_get_divide[i]-Pixel_get_divide[i+1])==0x55)
            {
                right_line=i;       //�������ֵ127 ��Сֵ1
                break;
            }
        }
        if((left_line!=0)&&(right_line!=127))      //�����߶����ҵ������
        {              
            // wide=right_line-left_line;         //�߿� 
            if((right_line<left_line)&&((left_line-right_line)<20))    //�ҵ�����
            {
                 direction_p=direction_p_s;    //5.9
                 direction_d=direction_d_s;   //0.1
                mid=(right_line+left_line)/2;
                single_flag=1;
                //speed_goal=85.0;
                gpio_set (PORTA, 5, 0);
                gpio_set (PORTA, 4, 0);
                gpio_set (PORTD, 0, 0);
            }
            else
            {
              single_flag=0;
              gpio_set (PORTA, 5, 1);
              gpio_set (PORTD, 0, 1);
              //gpio_set (PORTA, 4, 1);    
            }
          
        }
        else if((right_line==127)&&(left_line==0))          //������
            {
                mid=mid;       
            }
        else
        {
            single_flag=0;
            gpio_set (PORTA, 5, 1);
            gpio_set (PORTD, 0, 1);
            //gpio_set (PORTA, 4, 1);   
        }
        if(single_flag==0)
        {
            left_line=0;
            right_line=127;
            for(i=mid;i>0;i--)       //Ѱ�߷�Χ����~0
            {
                if(i>127)
                {
                    i=127;
                }
                if((Pixel_get_divide[i]-Pixel_get_divide[i-1])==0x55)
                {
                    left_line=i;        //������Сֵ1  ���ֵ127
                    break;
                }
    
            }
            for(i=mid;i<127;i++)    //Ѱ�߷�Χ����~127
            {
                if(i<0)
                {
                    i=0;
                }
                if((Pixel_get_divide[i]-Pixel_get_divide[i+1])==0x55)
                {
                    right_line=i;       //�������ֵ127 ��Сֵ1
                    break;
                }
            }
            if((left_line!=0)&&(right_line!=127))      //�����߶����ҵ������
            {    
                wide=right_line-left_line;         //
                
                if(wide<80)             //����Ϊֱ�Ǳ�־
                {
                   if(barrier_flag==0)           //�ϰ�
                   { 
                    mid=mid;
                    if((wide_old<(wide_old_old+6))&&(wide_old>(wide_old_old-6)))
                    {
                        if((wide<(wide_old+6))&&(wide>(wide_old-6)))
                        {
                            barrier_flag=1;
                        }
                    }
                    wide_old_old=wide_old;
                    wide_old=wide;
                   }
                   if(barrier_flag==1)
                   {
                    if((128-right_line)>left_line)
                    {
                    mid=((left_line+right_line)/2)-20;
                    }
                    else
                    {
                    mid=((left_line+right_line)/2)+20;
                    }
                   }
                }
                else                   //���ϰ�
                {
                    mid=(left_line+right_line)/2;
                }
                if(wide>120)       //����޷�
                {
                    wide=120;
                }
                if(wide<100)  ////108
                {
                   if(barrier_flag==0)
                   {
                    wide=100;
                   }
                   else                      //���ϰ���С���� ���
                   {
                    
                    wide=wide-barrier_wide;
                   }
                }         
            }      
            if((right_line==127)&&(left_line!=0))             //�Ҳ������ߵ����
            {
                mid=left_line+wide/2;
            }
            if((left_line==0)&&(right_line!=127))             //�Ҳ������ߵ����
            {
                mid=right_line-wide/2;
            } 
            else if((right_line==127)&&(left_line==0))          //������
            {
                mid=64;       
            }
            if(barrier_flag==1)     //�ϰ���ʱ
            {
                barrier_time++;
                if(barrier_time>30)
                {
                    barrier_time=0;
                    barrier_flag=0;
                    wide=122;
                }
            }
       }
    }
    
    if((yuansu_flag==1))       //ֱ�ǳ���
    { 
       direction_p=9.0;
       direction_d=2.8;
       if(blackline_flag==1)
       {
        speed_goal=speed_angle;
       }
            for(i=mid;i>0;i--)       //Ѱ�߷�Χ����~0
            {
                if(i>127)
                {
                    i=127;
                }
                if((Pixel_get_divide[i]-Pixel_get_divide[i-1])==0x55)
                {
                    left_line=i;        //������Сֵ1  ���ֵ127
                    break;
                }
    
            }
            for(i=mid;i<127;i++)    //Ѱ�߷�Χ����~127
            {
                if(i<0)
                {
                    i=0;
                }
                if((Pixel_get_divide[i]-Pixel_get_divide[i+1])==0x55)
                {
                    right_line=i;       //�������ֵ127 ��Сֵ1
                    break;
                }
            }
            if((left_line!=0)&&(right_line!=127))      //�����߶����ҵ������
            {    
                wide=right_line-left_line;         //�߿�
                if((mid==119)||(mid==9))
                {
                    angle_flag=1;
                }
                angle_num_l=0;
                angle_num_r=0;
                if(wide<90)             //����Ϊֱ�Ǳ�־
                {
                    mid=mid;
                }
                else
                {
                    mid=(left_line+right_line)/2;
                }
                if(wide>120)       //����޷�
                {
                    wide=120;
                }
                if(wide<100)
                {
                    wide=100;
                }          
            }      
            if((right_line==127)&&(left_line!=0))             //�Ҳ������ߵ����
            {
              if(angle_flag==0)
              {
                mid=89;//84//left_line+wide/2;
                if(mid==9)
                {
                    angle_flag=1;
                }
                angle_num_l=0;
                angle_num_r++;
                if(angle_num_r>10)
                {
                  mid=119;
                  //angle_num_r=0;  
                }
                 if(angle_num_r>50)
                {
                  angle_num_r=0;
                  //angle_flag=1;
                  //yuansu_flag=0; 
                }
              }
             else
             {
                 mid=left_line+wide/2;
             } 
            }
            if((left_line==0)&&(right_line!=127))             //�Ҳ������ߵ����
            {
              if(angle_flag==0)
              {
                mid=39;//44//right_line-wide/2;
                if(mid==119)
                {
                    angle_flag=1;
                }
                angle_num_r=0;
                angle_num_l++;
                if(angle_num_l>10)
                {
                  mid=9;
                  //angle_num_r=0;
                }
                if(angle_num_l>50)
                {
                  angle_num_l=0;
                }
              }
              else
              {
                  mid=right_line-wide/2;
              }
            } 
       
       
            else if((right_line==127)&&(left_line==0))          //������
            {
                mid=mid;       
            }
       
      
    }
        
    if(display_flag==1)      //ʵʱ��ʾ�����ݴ�����ı�־Ϊ1ʱִ��
    {
        for(i=0;i<128;i++)
        {
            Pixel_get_now[i]=Pixel_get[i];
        }
        mid_now=mid;
        
        display_flag=0;
        trans_flag=1;          //���ʹ���ݵ�����
    }
    direct_dir=(float)mid-mid_law;
    direct_gyro=ad_once(ADC1, SE15, ADC_12bit);
    //OutData[1]=direct_gyro;
    direct_gyro=direct_gyro_zero-direct_gyro;
    if(blackline_flag==1)                         //ֱ��ת���ж�
    {
      
      if(coder_intergral>3000)     //һ��6155.625
      {
        if((direct_gyro>400)||(direct_gyro<-400))
        {
            blackline_flag=2;
            coder_intergral_flag=0;
            coder_intergral=0.0;
            coder_intergral_l=0.0;
            coder_intergral_r=0.0;
            //yuansu_flag=0;
            gpio_set (PORTA, 5, 0);          
        }
      }
      
    }
    
    gap=direct_dir*direction_p+direct_gyro*direction_d/10.0;
    
    //OutData[2]=direct_gyro;
    OutData[3]=wide;
    OutData[1]=mid;
    OutData[0]=r_duiguan;
    OutData[2]=l_duiguan;
    //OutData[1]=black_pixel_num;
    if(xianshi==3)
    {
        OutPut_Data();
    } 
    direction_out_r=0.0-gap;
    direction_out_l=gap;
}
