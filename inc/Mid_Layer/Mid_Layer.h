#ifndef MID_LAYER_H
#define MID_LAYER_H

#include "main.h"

/*******************************************************************************************************************
* DELAY
********************************************************************************************************************/
void Mid_Delay_Init(void);
void Mid_Delay_Ms(uint32_t ms);
void Mid_Delay_Us(uint32_t us);

/*******************************************************************************************************************
* USART
********************************************************************************************************************/
void Mid_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate);
void Mid_Uart_Send_Char(USART_TypeDef *USARTx, char c);
void Mid_Uart_Send_String(USART_TypeDef *USARTx, char *s);

/*******************************************************************************************************************
* CAM BIEN SIEU AM
********************************************************************************************************************/
void Mid_Hcsr04_Init(void);
void Mid_Trig_Pulse(void);
uint32_t Mid_Read_Echo_Pulse(void);
uint32_t Mid_Hcsr04_Get_Distance(void);

/*******************************************************************************************************************
* REMOTE
********************************************************************************************************************/
void Mid_Ir_Remote_Init(void); // khoi tao ir remote
Ir_Cmd_e Mid_Ir_Remote_Get_Cmd(void); // luc bam nut cai remote thi no nhan CMD
void Mid_Isr_Pulse(void); 

/*******************************************************************************************************************
* MOTOR
********************************************************************************************************************/
void Mid_Motor_Init(void);
void Mid_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right);
void Mid_Motor_Move_Forward(void);
void Mid_Motor_Move_Backward(void);
void Mid_Motor_Turn_Left(void);
void Mid_Motor_Turn_Right(void);
void Mid_Motor_Stop(void);

/*******************************************************************************************************************
* PID
********************************************************************************************************************/
void Mid_Line_Sensor_Init(void);
void Mid_Line_Sensor_Read(Line_Sensor_Data_t *data);
void Mid_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit);
void Mid_Line_PID_Reset(Line_PID_t *pid);
int16_t Mid_Line_PID_Update(Line_PID_t *pid, int8_t error);
void Mid_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed);

/*******************************************************************************************************************
* LOGIC
********************************************************************************************************************/
void Mid_Logic_Init(void);
void Mid_Logic_Run(void);

#endif /* MID_LAYER_H */
