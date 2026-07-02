#include "App_Layer.h"
#include "Mid_Layer.h"

void App_Delay_Init(void)
{
	Mid_Delay_Init();
}

void App_Delay_Ms(uint32_t ms)
{
	Mid_Delay_Ms(ms);
}

void App_Delay_Us(uint32_t us)
{
	Mid_Delay_Us(us);
}

void App_Hcsr04_Init(void)
{
	Mid_Hcsr04_Init();
}

void App_Trig_Pulse(void)
{
	Mid_Trig_Pulse();
}

uint32_t App_Read_Echo_Pulse(void)
{
	return Mid_Read_Echo_Pulse();
}

uint32_t App_Hcsr04_Get_Distance(void)
{
	return Mid_Hcsr04_Get_Distance();
}

void App_Ir_Remote_Init(void)
{
	Mid_Ir_Remote_Init();
}

Ir_Cmd_e App_Ir_Remote_Get_Cmd(void)
{
	return Mid_Ir_Remote_Get_Cmd();
}

void App_Motor_Init(void)
{
	Mid_Motor_Init();
}

void App_Motor_Set_Speed(uint16_t speed_left, uint16_t speed_right)
{
	Mid_Motor_Set_Speed(speed_left, speed_right);
}

void App_Motor_Move_Forward(void)
{
	Mid_Motor_Move_Forward();
}

void App_Motor_Move_Backward(void)
{
	Mid_Motor_Move_Backward();
}

void App_Motor_Turn_Left(void)
{
	Mid_Motor_Turn_Left();
}

void App_Motor_Turn_Right(void)
{
	Mid_Motor_Turn_Right();
}

void App_Motor_Stop(void)
{
	Mid_Motor_Stop();
}

void App_Line_Sensor_Init(void)
{
	Mid_Line_Sensor_Init();
}

void App_Line_Sensor_Read(Line_Sensor_Data_t *data)
{
	Mid_Line_Sensor_Read(data);
}

void App_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit)
{
	Mid_Line_PID_Init(pid, kp, ki, kd, output_limit, integral_limit);
}

void App_Line_PID_Reset(Line_PID_t *pid)
{
	Mid_Line_PID_Reset(pid);
}

int16_t App_Line_PID_Update(Line_PID_t *pid, int8_t error)
{
	return Mid_Line_PID_Update(pid, error);
}

void App_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed)
{
	Mid_Line_PID_Drive_Compute_Speeds(forward_speed, turn_rate, left_speed, right_speed);
}

void App_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate)
{
	Mid_Uart_Init(USARTx, baudrate);
}

void App_Uart_Send_Char(USART_TypeDef *USARTx, char c)
{
	Mid_Uart_Send_Char(USARTx, c);
}

void App_Uart_Send_String(USART_TypeDef *USARTx, char *s)
{
	Mid_Uart_Send_String(USARTx, s);
}

void App_Logic_Init(void)
{
	Mid_Logic_Init();
}
void App_Logic_Run(void)
{
	Mid_Logic_Run();
}

