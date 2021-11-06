#ifndef _FOC1_C_
#define _FOC1_C_
		
		
		
#endif
#include "foc1.h"
#include "pid.h"


/************************************************************************************************** 
 Function				: clarke_calc;
 Description			: Clark transform;
						  a,b,c -> alpha,beta
 Call by				: functions in TBC;
 Input Variables		: CLARKE
 Output/Return Variables: CLARKE
 Subroutine Call		: NONE
 Reference				: NONE
**************************************************************************************************/
void clarke_calc(CLARKE *v)
{	
   v->ds = 0.666666667f * (v->as-(0.5f * v->bs)-(0.5f * v->cs));  //Alphe
   v->qs = (v->bs - v->cs) * (0.57735026918963f);               //Beta
}

/************************************************************************************************** 
 Function				: park_calc;
 Description			: Park transform;
						  alpha,beta -> d,q
 Call by				: functions in TBC;
 Input Variables		: PARK
 Output/Return Variables: PARK
 Subroutine Call		: NONE
 Reference				: NONE
**************************************************************************************************/
void park_calc(PARK *v)
{	
	float cos_ang,sin_ang;
	arm_sin_cos_f32(v->ang,&sin_ang,&cos_ang);
	/*----------------------------------------FOC加负号-----------------------------------------------*/
	v->de =((v->ds * cos_ang) + (v->qs * sin_ang));
	v->qe =((v->qs * cos_ang) - (v->ds * sin_ang));
}


/************************************************************************************************** 
 Function				: iclarke_calc;
 Description			: Inv Clark transform;
						  alpha,beta -> a,b,c
 Call by				: functions in TBC;
 Input Variables		: ICLARKE
 Output/Return Variables: ICLARKE
 Subroutine Call		: NONE
 Reference				: NONE
**************************************************************************************************/
void iclarke_calc(ICLARKE *v)
{	
   v->as = v->ds;
   v->bs = ((-0.5f) * v->ds) + ((0.86602540f) * v->qs);
   v->cs = ((-0.5f) * v->ds) - ((0.86602540f) * v->qs);
}

/************************************************************************************************** 
 Function				: ipark_calc;
 Description			: Inv Park transform;
						  d,q -> alpha,beta 
 Call by				: functions in TBC;
 Input Variables		: IPARK
 Output/Return Variables: IPARK
 Subroutine Call		: NONE
 Reference				: NONE
**************************************************************************************************/
void ipark_calc(IPARK *v)
{	
	float cos_ang,sin_ang;
	arm_sin_cos_f32(v->ang,&sin_ang,&cos_ang);
	v->ds = (v->de * cos_ang) - (v->qe * sin_ang);
	v->qs = (v->qe * cos_ang) + (v->de * sin_ang);
}

/**************************************************************************************************
 Function				: svgendq_calc;
 Description			: SV pwm generation;
 Call by				: functions in TBC;
 Input Variables		: Valpha,Vbeta,Vdc
 Output/Return Variables: a,b,c PWM ratio
 Subroutine Call		: NONE
 Reference				: NONE
**************************************************************************************************/
void svgendq_calc(SVGENDQ *v)
{

	float Va,Vb,Vc,t1,t2,a,b;
	uint32_t Sector = 0;  // Sector is treated as Q0 - independently with global Q
 
	float Sector3[2][2] = {{(0.8660f),(-0.5f)},{(0),(1)}};
	float Sector1[2][2] = {{(-0.8660f),(0.5f)},{(0.8660f),(0.5f)}};
	float Sector5[2][2] = {{(0),(1)},{(-0.8660f),(-0.5f)}};
	float Sector4[2][2] = {{(0),(-1)},{(-0.8660f),(0.5f)}};
	float Sector6[2][2] = {{(-0.8660f),(-0.5)},{(0.8660f),(-0.5)}};
	float Sector2[2][2] = {{(0.8660f),(0.5f)},{(0),(-1)}};

// Vdc feedforward
#if 0
	if(v->UdcIn == 0)
	{
		v->UdcIn = 1;
	}

	v->Ualfa = _IQdiv(v->UalfaIn,v->UdcIn);
	v->Ubeta = _IQdiv(v->UbetaIn,v->UdcIn);
#endif

#if 1
	v->Ualfa = v->UalfaIn;
	v->Ubeta = v->UbetaIn;
#endif

// Inverse clarke transformation
    Va = v->Ubeta;
    Vb = ((-0.5f) * v->Ubeta) + ((0.8660254f) * v->Ualfa);  // 0.8660254 = sqrt(3)/2
    Vc = ((-0.5f) * v->Ubeta) - ((0.8660254f) * v->Ualfa);  // 0.8660254 = sqrt(3)/2

// 60 degree Sector determination
    if (Va>(0))
       Sector = 1;
    if (Vb>(0))
       Sector = Sector + 2;
    if (Vc>(0))
       Sector = Sector + 4;

    //v->Sector_Flag = Sector * 2000;


// X,Y,Z (Va,Vb,Vc) calculations

    if (Sector==0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
    {
       v->Ta = 0.5f;
       v->Tb = 0.5f;
       v->Tc = 0.5f;
    }
    if (Sector==1)  // Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
    {
       t1 = (Sector1[0][0] * v->Ualfa) + (Sector1[0][1] * v->Ubeta);
       t2 = (Sector1[1][0] * v->Ualfa) + (Sector1[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Tb = 0.5f * (1-t1-t2);      // tbon = (1-t1-t2)/2
       v->Ta = v->Tb + t1;                             // taon = tbon+t1
       v->Tc = v->Ta + t2;                             // tcon = taon+t2
    }
    else if (Sector==2)  // Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
    {
       t1 = (Sector2[0][0] * v->Ualfa) + (Sector2[0][1] * v->Ubeta);
       t2 = (Sector2[1][0] * v->Ualfa) + (Sector2[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Ta = (0.5f) * (1-t1-t2);      // taon = (1-t1-t2)/2
       v->Tc = v->Ta+t1;                             // tcon = taon+t1
       v->Tb = v->Tc+t2;                             // tbon = tcon+t2
    }
    else if (Sector==3)  // Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
    {
       t1 = (Sector3[0][0] * v->Ualfa) + (Sector3[0][1] * v->Ubeta);
       t2 = (Sector3[1][0] * v->Ualfa) + (Sector3[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Ta = (0.5f) * (1-t1-t2);      // taon = (1-t1-t2)/2
       v->Tb = v->Ta+t1;                             // tbon = taon+t1
       v->Tc = v->Tb+t2;                             // tcon = tbon+t2
    }
    else if (Sector==4)  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
    {
       t1 = (Sector4[0][0] * v->Ualfa) + (Sector4[0][1] * v->Ubeta);
       t2 = (Sector4[1][0] * v->Ualfa) + (Sector4[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Tc = (0.5f) * (1-t1-t2);      // tcon = (1-t1-t2)/2
       v->Tb = v->Tc+t1;                             // tbon = tcon+t1
       v->Ta = v->Tb+t2;                             // taon = tbon+t2
    }
    else if (Sector==5)  // Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
    {
       t1 = (Sector5[0][0] * v->Ualfa) + (Sector5[0][1] * v->Ubeta);
       t2 = (Sector5[1][0] * v->Ualfa) + (Sector5[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Tb = (0.5f) * (1-t1-t2);      // tbon = (1-t1-t2)/2
       v->Tc = v->Tb+t1;                             // tcon = tbon+t1
       v->Ta = v->Tc+t2;                             // taon = tcon+t2
    }
    else if (Sector==6)  // Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
    {
       t1 = (Sector6[0][0] * v->Ualfa) + (Sector6[0][1] * v->Ubeta);
       t2 = (Sector6[1][0] * v->Ualfa) + (Sector6[1][1] * v->Ubeta);
       if((t1+t2)>(0.99f))
       {
           a = t1;
           b = t2;
          t1 = (a * 0.99f) / (a+b);
          t2 = (b * 0.99f) / (a+b);
       }
       v->Tc = (0.5f) * (1-t1-t2);      // tcon = (1-t1-t2)/2
       v->Ta = v->Tc+t1;                             // taon = tcon+t1
       v->Tb = v->Ta+t2;                             // tbon = taon+t2
    }

}
///*atan查表*****************************************************************/
//typedef   signed short int int16_t;
//typedef unsigned short int u16;
//#define M_2PI (2 * 3.1415926535897932384626433832795)
//#define M_PI 3.1415926535897932384626433832795
//#define M_PI_2 (3.1415926535897932384626433832795 / 2)
//#define	LUT_NUM		450	/*该值越大表精度越高,精度为atan(1/LUT_NUM)*/

//#define Rad_Precision 573

//u16 g_atan_tab[LUT_NUM];

//#define	ASIN_NUM		450
//u16 g_asin_tab[ASIN_NUM];

//void atan2_tab_init(void)  
//{
//	u16 i = 0;

//	for (i = 0; i < LUT_NUM; i++)
//	{
//		g_atan_tab[i] = (u16)(atan((float)i / (LUT_NUM - 1)) * Rad_Precision);
//	}
//}

///**
//* @brief	atan2查表法
//* @param[in] y
//* @param[in] x
//* @note 调用此函数之前需先调用atan2_tab_init()生成表
//* @see
//* @code
//* @retval	返回弧度值
//*/
//float atan2_tab_calc(float y, float x)
//{
//	float Y_X, Y_X_temp;
//	float result = 0;

//	Y_X = (x == 0) ? (float)x / y : (float)y / x;
//	Y_X_temp = Y_X;

//	if (y == 0)
//	{
//		if (x >= 0)
//			return 0;
//		else
//			return (M_PI);  //Return Pi
//	}
//	else if (x > 0 && y > 0)
//	{
//		//第一象限
//		Y_X = (Y_X <= 1) ? Y_X  : 1 / Y_X;
//		result = (Y_X_temp > 1)? M_PI_2 - (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision : (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision;
//	}
//	else if (x < 0 && y > 0)
//	{
//		//第二象限
//		Y_X = (Y_X < -1)? Y_X = -1 / Y_X : -Y_X;
//		result = (Y_X_temp >= -1) ? M_PI - (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision : //小于45°
//								M_PI_2 + (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision;
//	}
//	else if (x < 0 && y < 0)
//	{
//		//第三象限
//		Y_X = (Y_X <= 1) ? Y_X : 1 / Y_X;
//		result = (Y_X_temp <= 1) ? (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision - M_PI: -M_PI_2 - (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision;
//	}
//	else if(x > 0 && y < 0)
//	{
//		//第四象限
//		Y_X = (Y_X < -1) ? Y_X = -1 / Y_X : -Y_X;
//		result = (Y_X_temp >= -1) ? -(float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision : //小于45°
//								-(M_PI_2 - (float)g_atan_tab[(int16_t)(Y_X*(LUT_NUM - 1))] / Rad_Precision);
//	}
//	else if(x == 0 && y > 0)
//	{
//		result = M_PI_2;
//	}
//	else
//	{
//		result = -M_PI_2;
//	}

//	return result;
//}
///**************************************************************************************************
//void foc(float Adc_Samp[])
//0-直流电压1---输 出
//1~3-Va~Bc
//4~6-Ia~Ic
//**************************************************************************************************/
//const fp32 PID1[3] = {-0.025, 0, 0};
//const fp32 PID2[3] = {0.2, 0, 0};
//const fp32 PID3[3] = {-0.025, 0, 0};
//pid_type_def pid1,pid2,pid3;
//float Vpi=0;
////需要先处理偏置
//void foc(float Adc_Samp[])
//{
//	clarke.as=Adc_Samp[1]; clarke.bs=Adc_Samp[2]; clarke.cs=Adc_Samp[3];
//	clarke_calc(&clarke);           //V a,b,c -> alpha,beta
//	park.ang=atan2_tab_calc(clarke.qs,clarke.ds);  //V theta
//	
//	clarke.as=Adc_Samp[4]; clarke.bs=Adc_Samp[5]; clarke.cs=Adc_Samp[6];
//	clarke_calc(&clarke);           //I a,b,c -> alpha,beta
//	park.ds=clarke.ds; park.qs=clarke.qs;  
//	park_calc(&park);               //I alpha,beta -> d,q
////	id=park.de;
////	iq=park.qe;                     //id、iq
//	
//	ipark.qe+=PID_calc(&pid1, park.qe, 0);//q
//	Vpi+=PID_calc(&pid2, Adc_Samp[0],30);//目标Vdc 30
//	ipark.de+=PID_calc(&pid2, park.de, Vpi);//d
//	
//	ipark.ang=park.ang;
//	ipark_calc(&ipark);             //d,q -> alpha,beta 
//	
//	iclarke.ds=ipark.ds; iclarke.qs=ipark.qs;
//	iclarke_calc(&iclarke);         //alpha,beta -> a,b,c
//	
//	//Tri_compare(&duty_cycle,&iclarke); //三角波比较器
//	//得到duty_cycle结构体包含三个波形占空比
//}
