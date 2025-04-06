#include "atim.h"
#include "sys.h"
#include "stm32f10x.h"

void PWM_Init(void) 
{
    // 1. 初始化PA8和PB13引脚为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置PA8为TIM1_CH1输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置PB13为TIM8_CH1输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 2. 初始化Timer1为1 MHz PWM信号
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
   

    TIM_TimeBaseStructure.TIM_Period = 1439;       // 计数器周期为 72 MHz / (71 + 1) = 1 MHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;     // 预分频器为0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);



    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 700;  // 初始占空比为50%（35/71）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_Cmd(TIM1, ENABLE);

    // 3. 初始化Timer8为20 kHz PWM信号
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 3599;   // 计数器周期为 72 MHz / (3599 + 1) = 20 kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;   // 预分频器为0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1799;  // 初始占空比为50%（1799/3599）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);

    TIM_Cmd(TIM8, ENABLE);
}

void PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint16_t channel, uint16_t dutyCycle) {
    // 修改占空比
    TIMx->CCR1 = dutyCycle;
}

void Motor_Control(int speed, int direction) {
    // speed 范围 0-100 (占空比百分比)
    // direction: 1 -> 正转, 0 -> 反转

    int pwm1_duty = (speed * 1439) / 100;  // 将速度百分比转化为 1 MHz PWM 的占空比
    int pwm2_duty = (speed * 3599) / 100;  // 将速度百分比转化为 20 kHz PWM 的占空比

    // 根据方向来调整占空比
    if (direction == 1) {
        // 正转: PA8 高占空比, PB13 低占空比
        PWM_SetDutyCycle(TIM1, TIM_Channel_1, pwm1_duty); // 正转 PWM
        PWM_SetDutyCycle(TIM8, TIM_Channel_1, 3599 - pwm2_duty); // 反转 PWM（20kHz PWM）
    } else {
        // 反转: PA8 低占空比, PB13 高占空比
        PWM_SetDutyCycle(TIM1, TIM_Channel_1, 3599 - pwm1_duty);  // 反转 PWM
        PWM_SetDutyCycle(TIM8, TIM_Channel_1, pwm2_duty);  // 正转 PWM
    }
}

