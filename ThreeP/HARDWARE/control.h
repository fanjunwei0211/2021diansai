#ifndef CONTROL_H

#define CONTROL_H

#include "main.h"
#include "pid.h"
#include "key.h"
#include "math.h"
#include "dma.h"
#include "tim.h"
#include "dma.h"
#include "adc.h"
#include "foc1.h"


extern uint16_t Adc_Samp[];   //检测dma采样序列的值
extern uint16_t adc_buf[];
extern const fp32 PID_i1[3];
extern const fp32 PID_i2[3];
extern const fp32 PID_v1[3];
extern const fp32 PID_v2[3];
extern const fp32 PID_oV[3];
extern pid_type_def pid_i1,pid_i2,pid_v2,pid_v1,pid_oV;
//extern float PhaseA[],PhaseB[],PhaseC[];


//void Led_Flash(int time);
void Starttime(void);
void pid_param_init(void);
int spwm_pwm(int* pwmdata, int f, int phase);


#endif
