#include "Mid_Layer.h"

void Mid_Delay_Init(void)
{
	Ip_Delay_Init();
}

void Mid_Delay_Ms(uint32_t ms)
{
	Ip_Delay_Ms(ms);
}

void Mid_Delay_Us(uint32_t us)
{
	Ip_Delay_Us(us);
}

void Mid_Hcsr04_Init(void)
{
	Ip_Hcsr04_Init();
}

void Mid_Trig_Pulse(void)
{
	Ip_Trig_Pulse();
}

uint32_t Mid_Read_Echo_Pulse(void)
{
	return Ip_Read_Echo_Pulse();
}

uint32_t Mid_Hcsr04_Get_Distance(void)
{
	return Ip_Hcsr04_Get_Distance();
}

void Mid_Ir_Remote_Init(void)
{
	Ip_Ir_Remote_Init();
}

Ir_Cmd_e Mid_Ir_Remote_Get_Cmd(void)
{
	return Ip_Ir_Remote_Get_Cmd();
}

void Mid_Motor_Init(void)
{
	Ip_Motor_Init();
}

void Mid_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right)
{
	Ip_Motor_Set_Speed(speed_left, speed_right);
}

void Mid_Motor_Move_Forward(void)
{
	Ip_Motor_Move_Forward();
}

void Mid_Motor_Move_Backward(void)
{
	Ip_Motor_Move_Backward();
}

void Mid_Motor_Turn_Left(void)
{
	Ip_Motor_Turn_Left();
}

void Mid_Motor_Turn_Right(void)
{
	Ip_Motor_Turn_Right();
}

void Mid_Motor_Stop(void)
{
	Ip_Motor_Stop();
}

void Mid_Line_Sensor_Init(void)
{
	Ip_Line_Sensor_Init();
}

void Mid_Line_Sensor_Read(Line_Sensor_Data_t *data)
{
	Ip_Line_Sensor_Read(data);
}

void Mid_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit)
{
	Ip_Line_PID_Init(pid, kp, ki, kd, output_limit, integral_limit);
}

void Mid_Line_PID_Reset(Line_PID_t *pid)
{
	Ip_Line_PID_Reset(pid);
}

int16_t Mid_Line_PID_Update(Line_PID_t *pid, int8_t error)
{
	return Ip_Line_PID_Update(pid, error);
}

void Mid_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed)
{
	Ip_Line_PID_Drive_Compute_Speeds(forward_speed, turn_rate, left_speed, right_speed);
}

void Mid_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate)
{
	Ip_Uart_Init(USARTx, baudrate);
}

void Mid_Uart_Send_Char(USART_TypeDef *USARTx, char c)
{
	Ip_Uart_Send_Char(USARTx, c);
}

void Mid_Uart_Send_String(USART_TypeDef *USARTx, char *s)
{
	Ip_Uart_Send_String(USARTx, s);
}

void Mid_Logic_Init(void)
{
	Ip_Logic_Init();
}
void Mid_Logic_Run(void)
{
	Ip_Logic_Run();
}

