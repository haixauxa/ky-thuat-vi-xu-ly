#ifndef __HCSR04_H__
#define __HCSR04_H__

#include "stm32f10x.h"                  // Device header

void Ip_Hcsr04_Init(void);
void Ip_Trig_Pulse(void);
uint32_t Ip_Read_Echo_Pulse(void);
uint32_t Ip_Hcsr04_Get_Distance(void);

#endif /* __HCSR04_H__ */
