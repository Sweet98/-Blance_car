/***********************************************************************
 * 文件描述         ：
 * 头文件调用       ：
 * 变量描述         ：
 * 函数功能描述     ：
 * 作者             ：e芯7号
 * 完成时间         ： 
 * 修改时间及内容   ：
************************************************************************/
#include  "kalman.h"
volatile float QingJiao = 0;
volatile float Gyro_Data = 0;
float OutData[4] = { 0 };
void OutPut_Data();
//float Gyro_intergral=0;
float gyro_rate=0.0;
int power=0;
extern float gyro_zero;
float acc_o=1860;
float acc_zero=1860.0;   //      2420   1916    1640 
float acc_speed_zero=1860.0;
//float Gyro_intergral=0.0;
float acc_z_juedui=0;
extern int xianshi;                          //显示
void kalman_update(void)
{
	 
	  float Q =0.5,R = 20.0;     //R越小跟的越紧Q陀螺仪   R加速度计
	  static float RealData = 0,RealData_P = 0;
          static int i ;
	  float NowData = 0,NowData_P = 0;
	  float Kg = 0,gyroscope_rate = 0,accelerometer_angle=0;
	  float Acc_z = 0, Gyro = 0;
	 // static float his_acc = 0.0,his_accx = 0.0,his_accz = 0.0;
	  
	 // while(!ATD0STAT0_SCF);   //等待转换完成
        acc_zero=acc_o;
   	acc_speed_zero=acc_o;
   	Acc_z = ad_once(ADC0, SE13, ADC_12bit);     //z轴最小值107最大值141中间124 3点测
   	Gyro  = ad_once(ADC0, SE12, ADC_12bit);     //陀螺仪零偏值105
        if(i<100)
        {
            i++;
        }
        else
        {
            power = ad_once(ADC0, SE8, ADC_12bit);      //采集电池电压值
            i = 0;
        }
   	//uart_putchar (UART2, 0x03);
        //uart_putchar (UART2, 0xfc);
        //uart_putchar (UART2, Acc_z);
        //uart_putchar (UART2, Gyro);
        //uart_putchar (UART2, 0xfc);
        //uart_putchar (UART2, 0x03);

      	//OutData[0] = Gyro;
   	//OutData[3]=Acc_z;     //输出绝对的倾角
        OutData[3]=power;
        acc_z_juedui=Acc_z;
   	Acc_z = Acc_z - acc_speed_zero;       //1988中间平衡           
   	Gyro  = Gyro  - gyro_zero;
        
       /*if(Gyro>80.0) 
        {
          Gyro = 80.0;
        } 
  	else if(Gyro< -80.0)   
        {
          Gyro = -80.0;
        }*/
       Gyro_Data = Gyro;
       OutData[0] = Gyro_Data;
      // Gyro_intergral=Gyro_intergral+ Gyro_Data*0.01*(-15.34);    //陀螺仪积分，15.34 放大倍数  0.005 5毫秒
       //OutData[2]=Gyro_intergral;
       gyro_rate=-Gyro_Data;                          //加速度计的微分
   accelerometer_angle =Acc_z;      //
    OutData[1] = accelerometer_angle;
     
    gyroscope_rate = Gyro*1; //0.414        //参考电压3.3v 12位ADC 放大9.1倍 enc-03 0.67mv/deg./sec.
    
                                          //(3300/4096)/(0.67*9.1)*(3.14/180) =  0.0023
    
    NowData = RealData + gyroscope_rate*0.01*(-15.34);                 //1.预估计 X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k)
    NowData_P = sqrt(Q*Q+RealData_P*RealData_P);              //2.计算预估计协方差矩阵   P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k)
    Kg = sqrt(NowData_P*NowData_P/(NowData_P*NowData_P+R*R)); //3.计算卡尔曼增益矩阵 K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k))
    RealData = NowData + Kg*(accelerometer_angle - NowData);  //4.更新估计 X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1))
    RealData_P = sqrt((1-Kg)*NowData_P*NowData_P);            //5.计算更新后估计协防差矩阵 P(k|k) =（I-K(k)*H(k)）*P(k|k-1)
                                                              //Q是陀螺仪偏差R是加速度计偏差
    QingJiao =  RealData;  
    
    
    OutData[2] = QingJiao; 
    if(xianshi==1)
    {
        OutPut_Data();
    }
    
}
