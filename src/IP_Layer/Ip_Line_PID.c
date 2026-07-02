#include "Ip_Line_PID.h"
#include "Ip_Motor.h"

static float Clamp_Float(float value, float limit)
{
	if (value > limit)
	{
		return limit;
	}

	if (value < -limit)
	{
		return -limit;
	}

	return value;
}

void Ip_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->output_limit = output_limit;
	pid->integral_limit = integral_limit;
	pid->integral = 0.0f;
	pid->previous_error = 0.0f;
}

void Ip_Line_PID_Reset(Line_PID_t *pid)
{
	pid->integral = 0.0f;
	pid->previous_error = 0.0f;
}

int16_t Ip_Line_PID_Update(Line_PID_t *pid, int8_t error)
{
	float current_error = (float)error;
	float derivative; // dao ham
	float output; 		// output

	pid->integral += current_error; // ki cong don error 
	pid->integral = Clamp_Float(pid->integral, pid->integral_limit); // gioi han ki

	derivative = current_error - pid->previous_error; // kd = error hien tai - error truoc do

	output = (pid->kp * current_error) + (pid->ki * pid->integral) + (pid->kd * derivative);
	output = Clamp_Float(output, pid->output_limit);
	pid->previous_error = current_error;
	return (int16_t)(output - 0.5f);
}

/**********************************************************************************************************************
* turn_rate: toc do re
* forward_speed: base speed - toc do di thang
**********************************************************************************************************************/
void Ip_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed)
{
	int32_t left = (int32_t)forward_speed + (int32_t)turn_rate;
	int32_t right = (int32_t)forward_speed - (int32_t)turn_rate;

	if (left < 0)
	{
		left = 0;
	}
	else if (left > MOTOR_MAX_SPEED)
	{
		left = MOTOR_MAX_SPEED;
	}

	if (right < 0)
	{
		right = 0;
	}
	else if (right > MOTOR_MAX_SPEED)
	{
		right = MOTOR_MAX_SPEED;
	}

	*left_speed = (uint16_t)left;
	*right_speed = (uint16_t)right;
}
