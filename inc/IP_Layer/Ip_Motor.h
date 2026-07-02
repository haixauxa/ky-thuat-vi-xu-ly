#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"                  // Device header

#define MOTOR_MAX_SPEED 999
#define TURN_LEFT_90_MS 350
#define TURN_RIGHT_90_MS 350

void Ip_Motor_Init(void);
void Ip_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right);
void Ip_Motor_Move_Forward(void);
void Ip_Motor_Move_Backward(void);
void Ip_Motor_Turn_Left(void);
void Ip_Motor_Turn_Right(void);
void Ip_Motor_Stop(void);

#endif /* MOTOR_H */
