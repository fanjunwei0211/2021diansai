#ifndef FOC_H
#define FOC_H

#define SQRT_3 1.7320508f
#define BUS_VOLTAGE 13.0f
#define Ts 8200
#include "sys.h"

extern int Limit_Pluse_Max_Value;
typedef struct SVPWM_S
{
    float UAlpha;       //���룬��ֹ����ϵAlpha�ᶨ�ӵ�ѹ
    float UBeta;        //���룬��ֹ����ϵBeta�ᶨ�ӵ�ѹ
    float Ua;           //A��ѹ		
    float Ub;           //B��ѹ
    float Uc;           //C��ѹ
    uint16_t taOn;      //A��ʱ��
    uint16_t tbOn;      //B��ʱ��
    uint16_t tcOn;      //C��ʱ��
}SVPWM;

typedef struct CLARK_S
{
	int16_t Ia;     //���룬A�ඨ�ӵ���
	int16_t Ib;     //���룬B�ඨ�ӵ���
	int16_t Ic;     //���룬C�ඨ�ӵ���
	float Ua;           //A��ѹ		
	float Ub;           //B��ѹ
	float Uc;           //C��ѹ
	float Ud_in;        //�����ѹd
	float Uq_in;        //�����ѹq
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
    int16_t Id;         //�������ת����ϵ�µ�D����ֵ����
    int16_t Iq;         //�������ת����ϵ�µ�Q����ֵ����
    int16_t IAlpha;     //���룬��ֹ����ϵAlpha�ᶨ�ӵ���
    int16_t IBeta;      //���룬��ֹ����ϵBeta�ᶨ�ӵ���
    float Ud;           //�������ת����ϵ�µ�D����ֵ��ѹ
    float Uq;           //�������ת����ϵ�µ�Q����ֵ��ѹ
    float Theta;        //��ת����Ƕ�
    float ActId;        //ʵ��D�����
    float ActIq;        //ʵ��Q����� 
}PARK;

void Park_Calc(PARK *v);
void Anti_Park_Calc(PARK *v, SVPWM *pstrSvpwm);
void Clark_Calc(CLARK *v, PARK *u);
void Svpwm_Calc(SVPWM *pstrSvpwm);
void iclarke_calc(ICLARKE *v, SVPWM *pstrSvpwm);
void Clarke_dq_cal(CLARK *c,PARK *p);
#endif
