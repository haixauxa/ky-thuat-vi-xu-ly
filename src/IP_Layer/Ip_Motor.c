#include "Ip_Motor.h"
#include "Ip_Delay.h"

void Ip_Motor_Init(void)
{
  // Enable clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; // set chan IN1, IN2, IN3, IN4 deu dieu huong robot
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // chan xuat PWM dieu khien toc do dong co
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStruct); // PB6
  TIM_OC2Init(TIM4, &TIM_OCInitStruct); // PB7

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM4, ENABLE);

  TIM_Cmd(TIM4, ENABLE);
}

void Ip_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right)
{
	if (speed_left > MOTOR_MAX_SPEED) speed_left = MOTOR_MAX_SPEED;
	if (speed_right > MOTOR_MAX_SPEED) speed_right = MOTOR_MAX_SPEED;

  TIM4->CCR1 = speed_left; // banh trai - PB6
  TIM4->CCR2 = speed_right; // banh phai - PB7
}

	// Di thang
	// OUT1 bang 1 khi Pin 12 = 1
	// OUT2 bang 0 khi Pin 13 = 0
	// OUT3 bang 1 khi Pin 11 = 1
	// OUt4 bang 0 khi Pin 10 = 0

	// Di lui
	// OUT1 bang 1 khi Pin 12 = 0
	// OUT2 bang 0 khi Pin 13 = 1
	// OUT3 bang 1 khi Pin 11 = 0
	// OUt4 bang 0 khi Pin 10 = 1

	// Quay trai - Trai lui Phai tien
	// OUT1 bang 1 khi Pin 12 = 1
	// OUT2 bang 0 khi Pin 13 = 0
	// OUT3 bang 1 khi Pin 11 = 0
	// OUt4 bang 0 khi Pin 10 = 1

	// Quay phai - Trai tien Phai lui
	// OUT1 bang 1 khi Pin 12 = 0
	// OUT2 bang 0 khi Pin 13 = 1
	// OUT3 bang 1 khi Pin 11 = 1
	// OUt4 bang 0 khi Pin 10 = 0

void Ip_Motor_Move_Forward(void)
{
	// motor left move forward
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	
	// motor right move forward
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void Ip_Motor_Move_Backward(void)
{
	// motor left 
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	
	// motor right 
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void Ip_Motor_Turn_Left(void)
{
	// motor left - banh trai tien
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	
	// motor right - banh phai lui
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void Ip_Motor_Turn_Right(void)
{
	// motor left - banh trai lui
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	
	// motor right - banh phai tien
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

inline void Ip_Motor_Stop(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
}
