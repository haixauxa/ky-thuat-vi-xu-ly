#include "Ip_Hcsr04.h"
#include "Ip_Delay.h"

#define ECHO_TIMEOUT 40000

void Ip_Hcsr04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* Configuration GPIO */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Configuration Input Capture */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStruct.TIM_Period = 0xffff;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; /* PA8 */
	TIM_ICInitStruct.TIM_ICFilter = 7;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // bat canh len truoc
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInit(TIM1, &TIM_ICInitStruct);
	TIM_Cmd(TIM1, ENABLE);
}

// phat xung toi thieu 10us HIGH de bat dau gui lenh do khoang cach cho cam bien sieu am
void Ip_Trig_Pulse(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	Ip_Delay_Us(2);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	Ip_Delay_Us(10);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

static inline uint8_t Wait_Capture(uint32_t timeout) 
{
	while (!(TIM1->SR & TIM_SR_CC1IF) && --timeout);
	return timeout > 0;
}

// su dung input capture de do canh len sau do luu vao t_rise va do canh xuong luu vao t_fall 
// sau do return t_fall - t_rise
uint32_t Ip_Read_Echo_Pulse(void)
{
	uint32_t t_rise, t_fall;
	TIM1->CNT = 0;
	
	// Capture rising
	TIM1->CCER &= ~(1 << 1); // clear CC1P de capture canh len cua IC1
	TIM1->SR &= ~(1 << 1);
	if (!Wait_Capture(ECHO_TIMEOUT)) return 0; // doi cho den khi co capture canh len thi se set bit CC1 cua IC1 len 1
	t_rise = TIM1->CCR1;
	
	// Capture falling
	TIM1->CCER |= (1 << 1); // enable CC1P de capture tai canh xuong cua IC1
	TIM1->SR &= ~(1 << 1);
	if (!Wait_Capture(ECHO_TIMEOUT)) return 0; // doi cho den khi co capture canh xuong thi se set bit CC1 cua IC1 len 1
	t_fall = TIM1->CCR1;
	
	if (t_fall >= t_rise)
	{
		return (uint32_t)(t_fall - t_rise);
	}
	else
	{
		return (uint32_t)((0xffff - t_rise) + t_fall + 1);
	}
}
uint32_t Ip_Hcsr04_Get_Distance(void)
{
	TIM1->CNT = 0;
	TIM1->SR = 0;
	TIM1->CCER &= ~TIM_CCER_CC1P;
	Ip_Trig_Pulse();
	uint32_t pulse_with = Ip_Read_Echo_Pulse();
	return pulse_with / 58; // cm
}
