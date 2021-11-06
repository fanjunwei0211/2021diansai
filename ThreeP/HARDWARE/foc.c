#include "foc.h"
#include "arm_math.h"

uint8_t u8Sector=0;
int Limit_Pluse_Max_Value=8000;

//clark�任ģ�����clark�任�Ƚ���������ϵת���ɶ���̶�����ϵ�ı任��
void Clark_Calc(CLARK *v, PARK *u)
{
	u->IAlpha = 2.0/3*(v->Ia - 0.5*v->Ib - 0.5*v->Ic);
	u->IBeta  = (SQRT_3/3)*(v->Ib-v->Ic);
} 

//park�任ģ�����park�任�ȶ���̶�����ϵת���ɶ�����ת����ϵ�ı任��������ת����ϵ��Ϊ�˽�����ͬ����������ŵ�����ת�ص������롣
void Park_Calc(PARK *v)
{
	float Sinthe,Costhe;
	arm_sin_cos_f32(v->Theta,&Sinthe,&Costhe);
	v->Id = v->IAlpha*Costhe+v->IBeta*Sinthe;
	v->Iq = -v->IAlpha*Sinthe+v->IBeta*Costhe;
//    v->ActId = (float)(v->Id)*7.15*3.3/4096;//������ʵ�ʵ���11.8a
//    v->ActIq = (float)(v->Iq)*7.15*3.3/4096;//������ʵ�ʵ���11.8a
}

//������ѹdq����
void Clarke_dq_cal(CLARK *c,PARK *p)
{
	float Sinthe,Costhe;
	arm_sin_cos_f32(p->Theta,&Sinthe,&Costhe);
	c->Ud_in = -(2.0/3*(c->Ua - 0.5*c->Ub - 0.5*c->Uc)*Costhe+(SQRT_3/3)*(c->Ub-c->Uc)*Sinthe);
	c->Uq_in = -(-2.0/3*(c->Ua - 0.5*c->Ub - 0.5*c->Uc)*Sinthe+(SQRT_3/3)*(c->Ub-c->Uc)*Costhe);
}

//��park�任����park�任�ȶ�����ת����ϵת���ɶ���̶�����ϵ�ı任
void Anti_Park_Calc(PARK *v, SVPWM *pstrSvpwm)
{
    float Sinthe,Costhe;
    arm_sin_cos_f32(v->Theta,&Sinthe,&Costhe);
		pstrSvpwm->UAlpha = v->Ud*(float)Costhe - v->Uq*(float)Sinthe;//��Χ��-sqrt(2)��?+sqrt(2)
    pstrSvpwm->UBeta  = v->Ud*(float)Sinthe + v->Uq*(float)Costhe;//��Χ��-sqrt(2)��?+sqrt(2)
}


void iclarke_calc(ICLARKE *v, SVPWM *pstrSvpwm)
{	
	v->ds=pstrSvpwm->UAlpha;
	v->qs=pstrSvpwm->UBeta;
	v->as = v->ds*Ts;
	v->bs = (((-0.5) * v->ds) + ((0.86602540) * v->qs))*Ts;
	v->cs = (((-0.5) * v->ds) - ((0.86602540) * v->qs))*Ts;
}

void Svpwm_Calc(SVPWM *pstrSvpwm)
{

		uint8_t a,b,c;
		int32_t Tx,Ty,Tz;
		int32_t t1,t2;
		uint8_t Udc = BUS_VOLTAGE;
		/* �������¹�ʽȷ������ */
		pstrSvpwm->Ua = pstrSvpwm->UBeta;                                           // beta
		pstrSvpwm->Ub = (SQRT_3*pstrSvpwm->UAlpha/2.0f - pstrSvpwm->UBeta/2.0f);  // alpha*sqrt(3)-beta
		pstrSvpwm->Uc = (-SQRT_3*pstrSvpwm->UAlpha/2.0f - pstrSvpwm->UBeta/2.0f); // -alpha*sqrt(3)-beta
//		pstrSvpwm->Ua = pstrSvpwm->UAlpha;                                           // beta
//		pstrSvpwm->Ub = (-pstrSvpwm->UAlpha/2.0f + SQRT_3*pstrSvpwm->UBeta/2.0f);  // alpha*sqrt(3)-beta
//		pstrSvpwm->Uc = (-pstrSvpwm->UAlpha/2.0f - SQRT_3*pstrSvpwm->UBeta/2.0f); // -alpha*sqrt(3)-beta
		if(pstrSvpwm->Ua>0)
				a = 1;
		else
				a = 0;
		if(pstrSvpwm->Ub>0)
				b = 1;
		else
				b = 0;
		if(pstrSvpwm->Uc>0)
				c = 1;
		else
				c = 0;
		u8Sector = 4*c + 2*b + a;
		/* �������湫ʽ�����X��Y��Z ����TsΪTimer1_Period,UdcΪMOTOR_POWER*/
		Tx = SQRT_3*pstrSvpwm->Ua/Udc*Ts;//X=sqrt(3)*beta*Ts/Udc
		Ty = SQRT_3*pstrSvpwm->Ub/Udc*Ts;//Y=(sqrt(3)/2*beta+3/2*alpha)*Ts/Udc
		Tz = SQRT_3*pstrSvpwm->Uc/Udc*Ts;//Z=(sqrt(3)/2*beta-3/2*alpha)*Ts/Udc
			/* ����SVPWMռ�ձ� */
			switch(u8Sector)
			{
					case 1:
											t1 = -Ty;//Ut4
											t2 = -Tz;//Ut5
											if(Ts<(t1+t2))
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->tbOn = (Ts- t1 - t2)/4;          //Tbon = (1-t1-t2)/4
											pstrSvpwm->taOn = pstrSvpwm->tbOn + t1/2;     //Taon = Tbon + t1/2
											pstrSvpwm->tcOn = pstrSvpwm->taOn + t2/2;     //Tcon = Taon + t2/2
											break; //2������
					case 2:
											t1 = -Tz;//Ut4
											t2 = -Tx;//Ut5
											if(Ts<(t1+t2))
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->taOn = (Ts -t1 - t2)/4;										 //Taon = (1-t1-t2)/4
											pstrSvpwm->tcOn = pstrSvpwm->taOn + t1/2;              //Tcon = Taon + t1/2
											pstrSvpwm->tbOn = pstrSvpwm->tcOn + t2/2;              //Tbon = Tcon + t2/2
											break;//6������
					case 3:
											t1 = Ty;//Ut4
											t2 = Tx;//Ut6
											if(Ts<(t1+t2))
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->taOn = (Ts - t1 - t2)/4;										 //Taon = (1-t1-t2)/4
											pstrSvpwm->tbOn = pstrSvpwm->taOn + t1/2;              //Tbon = Taon + t1/2
											pstrSvpwm->tcOn = pstrSvpwm->tbOn + t2/2;              //Tcon = Tbon + t2/2
											break;//1������
					case 4:
											t1 = -Tx;//Ut1
											t2 = -Ty;//Ut3
											if(Ts<(t1+t2))
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->tcOn = (Ts - t1 - t2)/4;             //Tcon = (1-t1-t2)/4
											pstrSvpwm->tbOn = pstrSvpwm->tcOn + t1/2;         //Tbon = Tcon + t1/2
											pstrSvpwm->taOn = pstrSvpwm->tbOn + t2/2;         //Taon = Tbon + t2/2
											break;//4������
					case 5:
											t1 = Tx;//Ut2
											t2 = Tz;//Ut3
											if(Ts<(t1+t2))//����ֹ��������������Բ�ε�ѹʸ��ʧ�棬���Բ�ȡ������С
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->tbOn = (Ts - t1 - t2)/4;										//Tbon = (1-t1-t2)/4
											pstrSvpwm->tcOn = pstrSvpwm->tbOn + t1/2;              //Tcon = Tbon + t1/2
											pstrSvpwm->taOn = pstrSvpwm->tcOn + t2/2;              //Taon = Tcon + t2/2
											break;//3������
					case 6:
											t1 = Tz;//Ut1
											t2 = Ty;//Ut5
											if(Ts<(t1+t2))
											{
													t1 = t1*Ts/(t1+t2);
													t2 = t2*Ts/(t1+t2);
											}
											pstrSvpwm->tcOn = (Ts - t1 - t2)/4;										//Tcon = (1-t1-t2)/4
											pstrSvpwm->taOn = pstrSvpwm->tcOn + t1/2;              //Taon = Tcon + t1/2
											pstrSvpwm->tbOn = pstrSvpwm->taOn + t2/2;              //Tbon = Taon + t2/2
											break;//5������
					default:break;
			}
			if(t1==0 && t2==0)
			{
					pstrSvpwm->taOn = 0;
					pstrSvpwm->tbOn = 0;
					pstrSvpwm->tcOn = 0;
			}
			if(pstrSvpwm->taOn >= (Limit_Pluse_Max_Value))
					pstrSvpwm->taOn = Limit_Pluse_Max_Value;
			if(pstrSvpwm->tbOn >= (Limit_Pluse_Max_Value))
					pstrSvpwm->tbOn = Limit_Pluse_Max_Value;
			if(pstrSvpwm->tbOn >= (Limit_Pluse_Max_Value))
					pstrSvpwm->tbOn = Limit_Pluse_Max_Value;
}
