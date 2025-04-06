#ifndef __ATIM_H
#define __ATIM_H

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint16_t channel, uint16_t dutyCycle);
void Motor_Control(int speed, int direction);

#endif
