/**
 * *****************************************************
 * file: 				ir_remote.c
 *
 * Created on: 	Feb 27, 2026
 * author:   		haihbv
 * *****************************************************
 */

#include "Ip_Ir_Remote.h"
#include "Ip_Ring_Buffer.h"

#define IR_CMD_BUFFER_ELEM_CNT  10u

//uint8_t debug_cmd = 0;
//uint32_t debug_raw = 0;
//uint8_t debug_addr = 0;

// ring buffer - luu command da giai ma xong
static uint8_t buffer[IR_CMD_BUFFER_ELEM_CNT];
static Ring_Buffer_t ir_cmd_buffer = {
	.buffer = buffer,
	.size = IR_CMD_BUFFER_ELEM_CNT,
	.head = 0,
	.tail = 0
};

typedef union {
	struct {
		uint8_t cmd_inverted;
		uint8_t cmd;
		uint8_t addr_inverted;
		uint8_t addr;
	} decoded;
	uint32_t raw;
} Ir_Msg_t;

static volatile Ir_Msg_t ir_msg;

#define DMA_BUF_SIZE 64
static uint16_t dma_buf[DMA_BUF_SIZE];
static uint16_t dma_rd_idx = 0;
static uint16_t pulse_count = 0;

static void Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void Ir_Dma_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR1;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dma_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

static void Tim2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1; // 10us: moi tick la 10us 
	// 9ms = 9000us = 900 ticks
	// 13,5ms = 13500us = 1350 ticks
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// Input Capture configuration
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x7;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset); // moi lan co canh xuong thi timer reset ve 0
	
	// Enable DMA request on CC1
	TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);
	
	// Enable Timer
	TIM_Cmd(TIM2, ENABLE);
}

static inline bool Is_Msg_Valid(void)
{
	return ((ir_msg.decoded.addr ^ ir_msg.decoded.addr_inverted) == 0xFFu) \
      && ((ir_msg.decoded.cmd ^ ir_msg.decoded.cmd_inverted)  == 0xFFu);
}

Ir_Cmd_e Ip_Ir_Remote_Get_Cmd(void) {
	// doc du lieu tu DMA
	uint16_t dma_wr_idx = DMA_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5); 
	while (dma_rd_idx != dma_wr_idx) // con du lieu chua xu ly thi doc tiep
	{
		uint16_t pulse_duration = dma_buf[dma_rd_idx];
		dma_rd_idx = (dma_rd_idx + 1) % DMA_BUF_SIZE;
		
		uint32_t t = pulse_duration; // thoi gian giua 2 canh xuong
		
		pulse_count++; // dem so canh da nhan
		
		if (pulse_count == 1) {
			// reset frame
			ir_msg.raw = 0;
		}
		else if (pulse_count == 2) {
			// Start bit t = 13,5ms
			if (t < 1200 || t > 1500) { // sai reset lai
				pulse_count = 1;
				ir_msg.raw = 0;
			}
		}
		else if (pulse_count >= 3 && pulse_count <= 34) { // doc 32 bit
			// bit pulse: bit 0 - 1.125ms (112 ticks), bit 1 - 2.25ms (225 ticks)
			if (t > 500) { // > 5ms reset
				pulse_count = 1;
				ir_msg.raw = 0;
			} else { // ghep bit 
				ir_msg.raw <<= 1;
				ir_msg.raw += (t >= 150) ? 1 : 0; 
			}
		}
		
		if (pulse_count == 34) {
			if (Is_Msg_Valid() && !Ip_Ring_Buffer_Full(&ir_cmd_buffer)) {
//				debug_raw = ir_msg.raw;
//				debug_cmd = ir_msg.decoded.cmd;
//				debug_addr = ir_msg.decoded.addr;
				Ip_Ring_Buffer_Put(&ir_cmd_buffer, ir_msg.decoded.cmd);
			}
			ir_msg.raw = 0;
			pulse_count = 0; // ready for next frame
		}
	}

	Ir_Cmd_e cmd = IR_CMD_NONE;
	__disable_irq();
	if (!Ip_Ring_Buffer_Empty(&ir_cmd_buffer)) {
		cmd = (Ir_Cmd_e)Ip_Ring_Buffer_Get(&ir_cmd_buffer);
	}
	__enable_irq();
	return cmd;
}

void Ip_Ir_Remote_Init(void)
{
	ir_msg.raw = 0;
	pulse_count = 0;
	dma_rd_idx = 0;
	
	Gpio_Config();
	Ir_Dma_Config();
	Tim2_Config();
}


