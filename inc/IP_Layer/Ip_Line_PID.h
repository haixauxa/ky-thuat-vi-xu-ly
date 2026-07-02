#ifndef IP_LINE_PID_H
#define IP_LINE_PID_H

#include "stm32f10x.h"

typedef struct
{
	float kp; 													// phan ung theo error hien tai -> lech nhieu thi sua nhieu
	float ki; 													// cong don error theo thoi gian -> dung de sua loi lech lau
	float kd; 													// phan ung theo toc do thay doi loi -> giam rung/ overshoot
	float integral; 										// tong loi tich luy (error sum)
	float previous_error; 							// loi truoc do -> dung de tinh dao ham (kd)
	float output_limit;									// gioi han output PID -> tranh be lai nhanh
	float integral_limit;								// gioi han tich phan (ki) -> tranh tich luy qua nhieu gay mat kiem soat
} Line_PID_t;

void Ip_Line_PID_Init(Line_PID_t *pid, float kp, float ki, float kd, float output_limit, float integral_limit);
void Ip_Line_PID_Reset(Line_PID_t *pid);
int16_t Ip_Line_PID_Update(Line_PID_t *pid, int8_t error);
void Ip_Line_PID_Drive_Compute_Speeds(uint16_t forward_speed, int16_t turn_rate, uint16_t *left_speed, uint16_t *right_speed);

#endif /* IP_LINE_PID_H */
