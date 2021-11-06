#ifndef FOC_H
#define FOC_H

#define SQRT_3 1.7320508f
#define BUS_VOLTAGE 13.0f
#define Ts 8200
#include "sys.h"

extern int Limit_Pluse_Max_Value;
typedef struct SVPWM_S
{
    float UAlpha;       //输入，静止坐标系Alpha轴定子电压
    float UBeta;        //输入，静止坐标系Beta轴定子电压
    float Ua;           //A电压		
    float Ub;           //B电压
    float Uc;           //C电压
    uint16_t taOn;      //A相时间
    uint16_t tbOn;      //B相时间
    uint16_t tcOn;      //C相时间
}SVPWM;

typedef struct CLARK_S
{
	int16_t Ia;     //输入，A相定子电流
	int16_t Ib;     //输入，B相定子电流
	int16_t Ic;     //输入，C相定子电流
	float Ua;           //A电压		
	float Ub;           //B电压
	float Uc;           //C电压
	float Ud_in;        //输入电压d
	float Uq_in;        //输入电压q
}CLARK;

typedef struct {  float  as;  		/* Input: phase-a stator variable  */
				  float  bs;			/* Input: phase-b stator variable  */
				  float  cs;          /* Input: phase-c stator variable  */
				  float  ds;			/* Output: stationary d-axis stator variable  */
				  float  qs;			/* Output: stationary q-axis stator variable  */
			
		 	 	  void  (*calc)();	/* Pointer to calculation function */ 
				 } ICLARKE;	  

typedef struct PARK_S
{
    int16_t Id;         //输出，旋转坐标系下的D坐标值电流
    int16_t Iq;         //输出，旋转坐标系下的Q坐标值电流
    int16_t IAlpha;     //输入，静止坐标系Alpha轴定子电流
    int16_t IBeta;      //输入，静止坐标系Beta轴定子电流
    float Ud;           //输出，旋转坐标系下的D坐标值电压
    float Uq;           //输出，旋转坐标系下的Q坐标值电压
    float Theta;        //旋转坐标角度
    float ActId;        //实际D轴电流
    float ActIq;        //实际Q轴电流 
}PARK;

void Park_Calc(PARK *v);
void Anti_Park_Calc(PARK *v, SVPWM *pstrSvpwm);
void Clark_Calc(CLARK *v, PARK *u);
void Svpwm_Calc(SVPWM *pstrSvpwm);
void iclarke_calc(ICLARKE *v, SVPWM *pstrSvpwm);
void Clarke_dq_cal(CLARK *c,PARK *p);
#endif
