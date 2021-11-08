#include "control.h"

extern DMA_HandleTypeDef hdma_adc1;

#define LIMIT(input, min, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < min) \
        {                      \
            input = min;      \
        }                      \
    }
		


//PID变量及参数
////逆变器
//const fp32 PID1_Vd[3] = {0.001, 0.0007, 0};
//const fp32 PID1_Vq[3] = {0.001, 0.0007, 0};
//const fp32 PID1_Id[3] = {0.001, 0.0005, 0};
//const fp32 PID1_Iq[3] = {0.001, 0.0005, 0};
//PFC
const fp32 PID1_Vd[3] = {0.001, 0.0007, 0};
const fp32 PID1_Vq[3] = {0.001, 0.0007, 0};
const fp32 PID1_Id[3] = {0.0005, 0.00003, 0};//0.001 ,0.0002
const fp32 PID1_Iq[3] = {0.0005, 0.00003, 0};
//const fp32 PID1_Id[3] = {0.00001, 0, 0};
//const fp32 PID1_Iq[3] = {0.00001, 0, 0};
const fp32 PID_UP[3] = {0.5,5,0};
const fp32 PID_UO[3] = {0,0,0};
//pid1-->逆变器，V-->电压环，I-->电流环
pid_type_def pid1_Vq,pid1_Id,pid1_Vd,pid1_Iq,pid_up,pid_uo;

uint16_t Adc_Samp[8];    //检测dma采样序列的值
uint16_t adc_buf[80]; //dma采样取平均缓冲
CLARKE clarke,clarke_I;
PARK park,park_I;
ICLARKE iclarke,iclarke_I;
IPARK ipark,ipark_I;
SVGENDQ SV_dq,SV_dq_I;
float TUd=8,TUq=0,TId=-0.8,TIq=-0.001,TUp=35.2999992,TUo=0;    //TUd-->目标电压d，TUq-->目标电压q   8,0.4-->功率因数+++ 36 ----- 35.2999992
int pwm_max=8400;
int Uin[3]={0},Iin[3]={0};
float U_DCP=0, U_DCO=0;    //PFC整流电压，BUCK输出电压
long int U_DCP_sum=0,U_DCO_sum=0,Vpi=4000;
int t_ang=0,ang_flag=0;
int soft_flag=1;

int abs(int data)
{
	if(data > 0)
		return data;
	else 
		return -data;
}

void Starttime(void)
{
	HAL_TIM_Base_Start_IT(&htim1);    //20k----0.05ms
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);    
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);   //三相互补波
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);    
	HAL_TIMEx_PWMN_Start(&htim8,TIM_CHANNEL_2);   //三相互补波
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);    
	HAL_TIMEx_PWMN_Start(&htim8,TIM_CHANNEL_1);   //三相互补波
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);    
	HAL_TIMEx_PWMN_Start(&htim8,TIM_CHANNEL_3);   //三相互补波
//	HAL_ADC_Start_DMA((ADC_HandleTypeDef*)&hadc1, (uint32_t*)adc_buf, (uint32_t)80);
	HAL_ADC_Start_DMA((ADC_HandleTypeDef*)&hadc1, (uint32_t*)Adc_Samp, (uint32_t)8);
	ipark.ang=90;
}

void pid_param_init()
{
	PID_init(&pid1_Vd,PID_DELTA,PID1_Vd, 0.85, 0, 0);
	PID_init(&pid1_Vq,PID_DELTA,PID1_Vq, 0.85, 0, 0);
	
	PID_init(&pid1_Id,PID_DELTA,PID1_Id, 0.9, 0, 0);
	PID_init(&pid1_Iq,PID_DELTA,PID1_Iq, 0.9, 0, 0);

	PID_init(&pid_up,PID_DELTA,PID_UP, 8000, 0, 0);  
	PID_init(&pid_uo,PID_DELTA,PID_UO, 0.85, 0, 0);
}


void Svpwm_Set_t(SVGENDQ *v)
{
	TIM8->CCR1 = v->Ta*pwm_max;
	TIM8->CCR2 = v->Tb*pwm_max;
	TIM8->CCR3 = v->Tc*pwm_max;
//		TIM8->CCR1 = (v->Ta)*pwm_max;
//	TIM8->CCR2 = (v->Tb)*pwm_max;
//	TIM8->CCR3 = (v->Tc)*pwm_max;
//	TIM1->CCR1 = v->Tc*pwm_max;
}

void Led_Flash(int time)
{
	  static int temp;
	  if(0==time) HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
	  else		if(++temp==time)	{HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);temp=0;}
}


//参数：com 为采样的原始数值
//返回值：iData 经过一阶滤波后的采样值
//限幅滤波算法

 
char filter(char Old_value,char New_Value)
{ 
	if ( ( New_Value- Old_value> 3000 ) || ( Old_value- New_Value> 3000 )) 
					return Old_value;
	else return New_Value;
}

int lowV( int com )
{
    static int iLastData;    //上一次值
    int iData;               //本次计算值
	
    float dPower = 0.9;               //滤波系数
//		if(com >= 4096 || com <=0)
			
    iData = ( com * dPower ) + ( 1 - dPower ) * iLastData; //计算
//		iData = filter(iLastData,iData);
    iLastData = iData;                                     //存贮本次数据
    return iData;                                         //返回数据
}


//电网电流锁相逆变器
//Adc_Samp[0]-->Ua Adc_Samp[1]-->Ub Adc_Samp[2]-->Uc	
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	static int dc_delay=0,delay_cnt=0,last_ang=0;
	if(htim==&htim1)    //0.05ms   20kHz
	{
		dc_delay++;
		delay_cnt++;
		
//		Led_Flash(5000);  //250ms
		//采样计算
		//电网电压
		Uin[0]=lowV(Adc_Samp[0]);Uin[1]=lowV(Adc_Samp[1]);Uin[2]=lowV(Adc_Samp[2]);//有低通滤波
		clarke.as=(Uin[0]-1861)*3.3f*36/4095;
		clarke.bs=(Uin[1]-1861)*3.3f*36/4095;
		clarke.cs=(Uin[2]-1861)*3.3f*36/4095;
		clarke_calc(&clarke);//ABC-->ALPHA,BETA		
		park.ds=clarke.ds;//ALPHA
		park.qs=clarke.qs;//BETA
		
		if(park.ds>0)
			park.ang=atanf(park.qs/park.ds)/PI*180;
		else if(park.ds<0 && park.qs>0)
			park.ang=atanf(park.qs/park.ds)/PI*180+180;
		else if(park.ds<0 && park.qs<0)
			park.ang=atanf(park.qs/park.ds)/PI*180-180;
		ipark_I.ang=park_I.ang=park.ang;
		
//		if(delay_cnt>=400)
//		{
//			
//			if(last_ang-park.ang > -0.5 && last_ang-park.ang < 0.5 ) ang_flag=1;
//			last_ang=park.ang;
//			delay_cnt=0;
//		}
//		if(ang_flag == 0 && park.ang<90.5 && park.ang>89.5)
//		{
//				ang_flag = 1;
//		}
//		if(ang_flag == 1)
//		{
//			ipark.ang-=360.0f/400;
//			ipark_I.ang=park_I.ang=ipark.ang;
//			if(ipark.ang>=180) ipark_I.ang=park_I.ang=ipark.ang=-180;
//		}
		
		Iin[0]=lowV(Adc_Samp[3]);Iin[1]=lowV(Adc_Samp[4]);Iin[2]=lowV(Adc_Samp[5]);//有低通滤波
		
		clarke_I.as=(Iin[0]-1861)*3.3f*36/4095;
		clarke_I.bs=(Iin[1]-1861)*3.3f*36/4095;
		clarke_I.cs=(Iin[2]-1861)*3.3f*36/4095;
		clarke_calc(&clarke_I);//ABC-->ALPHA,BETA		
		park_I.ds=clarke_I.ds;
		park_I.qs=clarke_I.qs;
		
		//采样计算		
		park_calc(&park_I);//采样的d,q计算 ALPHA,BETA-->d,q		
//		ipark_I.de=PID_calc(&pid1_Id,-park_I.de,TId);
//		ipark_I.qe=PID_calc(&pid1_Iq,-park_I.qe,TIq);
		//PFC输出电压闭环
		U_DCP_sum+=Adc_Samp[7];
		if(dc_delay >= 25)
		{
			U_DCP = U_DCP_sum/25*3.3/4095*12;
//			Vpi = PID_calc(&pid_up,U_DCP,TUp);
			if(Vpi<0) Vpi=0;
			TIM1->CCR1 = Vpi;
			U_DCP_sum=0;dc_delay=0;
		}
		
		ipark_calc(&ipark_I);
		SV_dq_I.UalfaIn=ipark_I.ds;
		SV_dq_I.UbetaIn=ipark_I.qs;
		svgendq_calc(&SV_dq_I);
//		if(soft_flag)
//		{
//			LIMIT(SV_dq_I.Ta, 0.6, 0.9);
//			LIMIT(SV_dq_I.Tb, 0.6, 0.9);
//			LIMIT(SV_dq_I.Tc, 0.6, 0.9);
//		}
		Svpwm_Set_t(&SV_dq_I);
	}
}



#define CONTROL


////逆变控制
////Adc_Samp[0]-->Ua Adc_Samp[1]-->Ub Adc_Samp[2]-->Uc	
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{	
//	if(htim==&htim1)    //0.05ms   20kHz
//	{
//		Led_Flash(5000);  //250ms
//		//采样计算
////		Uin[0]=Adc_Samp[0];Uin[1]=Adc_Samp[1];Uin[2]=Adc_Samp[2];//无低通滤波
//#ifdef UCONTROL
//		Uin[0]=lowV(Adc_Samp[0]);Uin[1]=lowV(Adc_Samp[1]);Uin[2]=lowV(Adc_Samp[2]);//有低通滤波
//		
//		clarke.as=(Uin[0]-1861)*3.3f*36/4095;
//		clarke.bs=(Uin[1]-1861)*3.3f*36/4095;
//		clarke.cs=(Uin[2]-1861)*3.3f*36/4095;
//		clarke_calc(&clarke);//ABC-->ALPHA,BETA		
//		park.ds=clarke.ds; //alpha
//		park.qs=clarke.qs; //beta
//#else
//		Iin[0]=lowV(Adc_Samp[3]);Iin[1]=lowV(Adc_Samp[6]);Iin[2]=lowV(Adc_Samp[5]);//有低通滤波
//		
//		clarke_I.as=(Iin[0]-1861)*3.3f*36/4095;
//		clarke_I.bs=(Iin[1]-1861)*3.3f*36/4095;
//		clarke_I.cs=(Iin[2]-1861)*3.3f*36/4095;
//		clarke_calc(&clarke_I);//ABC-->ALPHA,BETA		
//		park_I.ds=clarke_I.ds;
//		park_I.qs=clarke_I.qs;
//#endif
//		//采样计算
//		
//		
//#ifdef UCONTROL	
//		ipark.ang+=360.0f/400;
//		park.ang=ipark.ang;
//		if(ipark.ang>=36000000) ipark.ang=0;
//		
//		park_calc(&park);//采样的d,q计算 ALPHA,BETA-->d,q		
//		ipark.de=PID_calc(&pid1_Vd,-park.de,TUd);
//		ipark.qe=PID_calc(&pid1_Vq,-park.qe,TUq);
//		ipark_calc(&ipark);
//		SV_dq.UalfaIn=ipark.ds;
//		SV_dq.UbetaIn=ipark.qs;
//		svgendq_calc(&SV_dq);
//		Svpwm_Set_t(&SV_dq);
//#else
//		ipark_I.ang+=360.0f/400;
//		park_I.ang=ipark_I.ang;
//		if(ipark_I.ang>=36000000) ipark_I.ang=0;
//		
//		park_calc(&park_I);//采样的d,q计算 ALPHA,BETA-->d,q		
//		ipark_I.de=PID_calc(&pid1_Id,park_I.de,TId);
//		ipark_I.qe=PID_calc(&pid1_Iq,park_I.qe,TIq);
//		ipark_calc(&ipark_I);
//		SV_dq_I.UalfaIn=ipark_I.ds;
//		SV_dq_I.UbetaIn=ipark_I.qs;
//		svgendq_calc(&SV_dq_I);
//		Svpwm_Set_t(&SV_dq_I);
//#endif
//	}
//}




////PFC控制
////Adc_Samp[0]-->Ua Adc_Samp[1]-->Ub Adc_Samp[2]-->Uc	
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{	
//	if(htim==&htim1)    //0.05ms   20kHz
//	{
////		Led_Flash(5000);  //250ms
//		//采样计算
//		//电网电压
//		Uin[0]=lowV(Adc_Samp[0]);Uin[1]=lowV(Adc_Samp[1]);Uin[2]=lowV(Adc_Samp[2]);//有低通滤波
//		clarke.as=(Uin[0]-1861)*3.3f*36/4095;
//		clarke.bs=(Uin[1]-1861)*3.3f*36/4095;
//		clarke.cs=(Uin[2]-1861)*3.3f*36/4095;
//		clarke_calc(&clarke);//ABC-->ALPHA,BETA		
//		park.ds=clarke.ds;//ALPHA
//		park.qs=clarke.qs;//BETA
//		park_calc(&park);//电网电压采样的d,q计算 ALPHA,BETA-->d,q	
//		if(park.ds>0)
//			park.ang=atanf(park.qs/park.ds)/PI*180;
//		else if(park.ds<0 && park.qs>0)
//			park.ang=atanf(park.qs/park.ds)/PI*180+180;
//		else if(park.ds<0 && park.qs<0)
//			park.ang=atanf(park.qs/park.ds)/PI*180-180;
//		ipark_I.ang=park_I.ang=park.ang;
//		
//		//电感电流
//		Iin[0]=lowV(Adc_Samp[3]);Iin[1]=lowV(Adc_Samp[6]);Iin[2]=lowV(Adc_Samp[5]);//有低通滤波
//		clarke_I.as=(Iin[0]-1861)*3.3f*36/4095;
//		clarke_I.bs=(Iin[1]-1861)*3.3f*36/4095;
//		clarke_I.cs=(Iin[2]-1861)*3.3f*36/4095;
//		clarke_calc(&clarke_I);//ABC-->ALPHA,BETA		
//		park_I.ds=clarke_I.ds;//ALPHA
//		park_I.qs=clarke_I.qs;//BETA
//		park_calc(&park_I);//电感电流采样的d,q计算 ALPHA,BETA-->d,q		
//				
//		
////		ipark_I.de=PID_calc(&pid1_Id,-park_I.de,TId);
////		ipark_I.qe=PID_calc(&pid1_Iq,-park_I.qe,TIq);
//		ipark_calc(&ipark_I);
//		SV_dq_I.UalfaIn=ipark_I.ds;//ALPHA
//		SV_dq_I.UbetaIn=ipark_I.qs;//BETA
//		svgendq_calc(&SV_dq_I);
//		Svpwm_Set_t(&SV_dq_I);
//	}
//}


