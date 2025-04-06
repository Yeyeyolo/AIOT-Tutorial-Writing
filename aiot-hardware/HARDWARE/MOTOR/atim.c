#include "atim.h"
#include "sys.h"
#include "stm32f10x.h"

void PWM_Init(void) 
{
    // 1. ��ʼ��PA8��PB13����Ϊ�����������
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // ����PA8ΪTIM1_CH1���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ����PB13ΪTIM8_CH1���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 2. ��ʼ��Timer1Ϊ1 MHz PWM�ź�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
   

    TIM_TimeBaseStructure.TIM_Period = 1439;       // ����������Ϊ 72 MHz / (71 + 1) = 1 MHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;     // Ԥ��Ƶ��Ϊ0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);



    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 700;  // ��ʼռ�ձ�Ϊ50%��35/71��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_Cmd(TIM1, ENABLE);

    // 3. ��ʼ��Timer8Ϊ20 kHz PWM�ź�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 3599;   // ����������Ϊ 72 MHz / (3599 + 1) = 20 kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;   // Ԥ��Ƶ��Ϊ0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1799;  // ��ʼռ�ձ�Ϊ50%��1799/3599��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);

    TIM_Cmd(TIM8, ENABLE);
}

void PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint16_t channel, uint16_t dutyCycle) {
    // �޸�ռ�ձ�
    TIMx->CCR1 = dutyCycle;
}

void Motor_Control(int speed, int direction) {
    // speed ��Χ 0-100 (ռ�ձȰٷֱ�)
    // direction: 1 -> ��ת, 0 -> ��ת

    int pwm1_duty = (speed * 1439) / 100;  // ���ٶȰٷֱ�ת��Ϊ 1 MHz PWM ��ռ�ձ�
    int pwm2_duty = (speed * 3599) / 100;  // ���ٶȰٷֱ�ת��Ϊ 20 kHz PWM ��ռ�ձ�

    // ���ݷ���������ռ�ձ�
    if (direction == 1) {
        // ��ת: PA8 ��ռ�ձ�, PB13 ��ռ�ձ�
        PWM_SetDutyCycle(TIM1, TIM_Channel_1, pwm1_duty); // ��ת PWM
        PWM_SetDutyCycle(TIM8, TIM_Channel_1, 3599 - pwm2_duty); // ��ת PWM��20kHz PWM��
    } else {
        // ��ת: PA8 ��ռ�ձ�, PB13 ��ռ�ձ�
        PWM_SetDutyCycle(TIM1, TIM_Channel_1, 3599 - pwm1_duty);  // ��ת PWM
        PWM_SetDutyCycle(TIM8, TIM_Channel_1, pwm2_duty);  // ��ת PWM
    }
}

