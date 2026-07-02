/**
 * *****************************************************
 * @file 		ir_remote.h
 *
 * @date 		Created on: Feb 27, 2026
 * @author   	haihbv
 * *****************************************************
 */

#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#include "stm32f10x.h"                  // Device header
#include "assert.h"
#include "stdbool.h"

/*****************************************************************************
* NEC Protocol
* 1 start frame: 9ms LOW + 4,5ms HIGH
* 32 bit-data:
*				- address (8-bit)
*				- address inverted (8 bit)
*				- command (8-bit)
*				- command inverted (8bit)
* bit 0 = 1.125ms
* bit 1 = 2.25ms
******************************************************************************/

typedef enum
{
	IR_CMD_0 = 0x98,
	IR_CMD_1 = 0xA2,
	IR_CMD_2 = 0x62,
	IR_CMD_3 = 0xE2,
	IR_CMD_4 = 0x22,
	IR_CMD_5 = 0x02,
	IR_CMD_6 = 0xC2,
	IR_CMD_7 = 0xE0,
	IR_CMD_8 = 0xA8,
	IR_CMD_9 = 0x90,
	IR_CMD_STAR = 0x68,
	IR_CMD_HASH = 0xB0,
	IR_CMD_UP = 0x18,
	IR_CMD_LEFT = 0x10,
	IR_CMD_RIGHT = 0x5A,
	IR_CMD_DOWN = 0x4A,
	IR_CMD_OK = 0x38,
	IR_CMD_NONE = 0xFF,
} Ir_Cmd_e;

void Ip_Ir_Remote_Init(void); // khoi tao ir remote
Ir_Cmd_e Ip_Ir_Remote_Get_Cmd(void); // luc bam nut cai remote thi no nhan CMD
void Ip_Isr_Pulse(void); 
void TIM2_IRQHandler(void);
void EXTI0_IRQHandler(void);

#endif /* IR_REMOTE_H */
