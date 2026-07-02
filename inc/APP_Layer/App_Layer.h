#ifndef APP_LAYER_H
#define APP_LAYER_H

#include "main.h"

void App_Delay_Init(void);
void App_Delay_Ms(uint32_t ms);
void App_Delay_Us(uint32_t us);

void App_Hcsr04_Init(void);
void App_Trig_Pulse(void);
uint32_t App_Read_Echo_Pulse(void);
uint32_t App_Hcsr04_Get_Distance(void);

void App_Ir_Remote_Init(void); // khoi tao ir remote
Ir_Cmd_e App_Ir_Remote_Get_Cmd(void); // luc bam nut cai remote thi no nhan CMD

void App_Motor_Init(void);
void App_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right);
void App_Motor_Move_Forward(void);
void App_Motor_Move_Backward(void);
void App_Motor_Turn_Left(void);
void App_Motor_Turn_Right(void);
void App_Motor_Stop(void);

void App_Line_Sensor_Init(void);
void App_Line_Sensor_Read(Line_Sensor_Data_t *data);

void App_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit);
void App_Line_PID_Reset(Line_PID_t *pid);
int16_t App_Line_PID_Update(Line_PID_t *pid, int8_t error);
void App_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed);

void App_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate);
void App_Uart_Send_Char(USART_TypeDef *USARTx, char c);
void App_Uart_Send_String(USART_TypeDef *USARTx, char *s);

void App_Logic_Init(void);
void App_Logic_Run(void);

#endif /* APP_LAYER_H */
