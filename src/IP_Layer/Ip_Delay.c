#include "stm32f10x.h"
#include "Ip_Delay.h"

Delay_Struct_t delay;
static volatile unsigned long systick_counter = 0;

#define STK_CTRL 			(*(volatile uint32_t *)(0xE000E010 + 0x00))
#define STK_LOAD  		(*(volatile uint32_t *)(0xE000E010 + 0x04))
#define STK_VAL 			(*(volatile uint32_t *)(0xE000E010 + 0x08))
#define STK_CALIB 		(*(volatile uint32_t *)(0xE000E010 + 0x0C))

void Ip_Delay_Init(void)
{
	/*
	* 1. clksource: processor clock (AHB)
	* 2. counting down to zero send request to Systick exception
	* 3. enable counter be used to:
			 - reload from load register
			 - counting down
			 - when it count down to 0 generate interrupt 
	*/
	STK_CTRL |= (1 << 0) | (1 << 1) | (1 << 2);
	STK_VAL = 0;
	STK_LOAD = 72000 - 1; // sinh ngat sau 72000 => ms
}

unsigned long Ip_Get_Tick(void)
{
	uint32_t tick;
	
	__disable_irq();
	tick = systick_counter;
	__enable_irq();
	
	return tick;
}

void Ip_Delay_Ms(uint32_t ms)
{
	uint32_t tick_start = Ip_Get_Tick();
	while (Ip_Get_Tick() - tick_start < ms);
}

void Ip_Delay_Us(uint32_t us)
{
	STK_LOAD = 72 - 1;
	uint32_t i;
	for (i = 0; i < us; i++)
	{
		STK_VAL = 0;
		
		while (!(STK_CTRL & (1 << 16)));
	}
	STK_LOAD = 72000 - 1;
	STK_VAL = 0;
}

void Delay_Struct_Init(void) __attribute__((constructor));

void Delay_Struct_Init(void)
{
	delay.init = Ip_Delay_Init;
}
void SysTick_Handler(void)
{
	++systick_counter;
}

