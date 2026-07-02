#include "Ip_Usart.h"
#include "stdio.h"

Uart_Struct_t uart1;
Uart_Struct_t uart2;
Uart_Struct_t uart3;

#define DEBUG_USART uart1

struct __FILE {
	int handle;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{
	(void)f;
	if (ch == '\n') {
		DEBUG_USART.send_char('\r');
	}
	DEBUG_USART.send_char((char)ch);
	return ch;
}

void Ip_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// gpio, clock for USART
	if (USARTx == USART1)
	{
		// TX: PA9 , RX: PA10
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	} else if (USARTx == USART2)
	{
		// TX: PA2, RX: PA3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	} else if (USARTx == USART3)
	{
		// TX: PB10, RX: PB11
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	
	// config USART
	if (USARTx == USART1)
	{
		USARTx->BRR = (( SystemCoreClock + baudrate / 2U ) / baudrate);
	}
	else 
	{
		USARTx->BRR = (( SystemCoreClock / 2 + baudrate / 2U ) / baudrate);
	}
	USARTx->CR1 |= (0 << 12); //  1 Start bit, 8 Data bits, n Stop bit
	USARTx->CR1 |= (0 << 10); // 	0: Parity control disabled
	USARTx->CR1 |= (0 << 9); //// even parity
	USARTx->CR1 |= ( (1 << 3) | (1 << 2) ); // enable TX & RX
	
	USARTx->CR2 |= (0 << 12);
	
	USARTx->CR1 |= (1 << 13); // enable usart
}

void Ip_Uart_Send_Char(USART_TypeDef *USARTx, char c)
{
	while (!(USARTx->SR & (1 << 7)));
	USARTx->DR = c;
}

void Ip_Uart_Send_String(USART_TypeDef *USARTx, char *s)
{
	while (*s)
	{
		Ip_Uart_Send_Char(USARTx, *s++);
	}
}

// static
void Ip_Uart1_Init(uint32_t baudrate) { Ip_Uart_Init(USART1, baudrate); }
void Ip_Uart1_Send_Char(char c) { Ip_Uart_Send_Char(USART1, c); };
void Ip_Uart1_Send_String(char *s) { Ip_Uart_Send_String(USART1, s); };

void Ip_Uart2_Init(uint32_t baudrate) { Ip_Uart_Init(USART2, baudrate); }
void Ip_Uart2_Send_Char(char c) { Ip_Uart_Send_Char(USART2, c); };
void Ip_Uart2_Send_String(char *s) { Ip_Uart_Send_String(USART2, s); };

void Ip_Uart3_Init(uint32_t baudrate) { Ip_Uart_Init(USART3, baudrate); }
void Ip_Uart3_Send_Char(char c) { Ip_Uart_Send_Char(USART3, c); };
void Ip_Uart3_Send_String(char *s) { Ip_Uart_Send_String(USART3, s); };

void Uart_Struct_Init(void) __attribute__((constructor));

void Uart_Struct_Init(void)
{
	uart1.init = Ip_Uart1_Init;
	uart1.send_char = Ip_Uart1_Send_Char;
	uart1.send_string = Ip_Uart1_Send_String;
	
	uart2.init = Ip_Uart2_Init;
	uart2.send_char = Ip_Uart2_Send_Char;
	uart2.send_string = Ip_Uart2_Send_String;
	
	uart3.init = Ip_Uart3_Init;
	uart3.send_char = Ip_Uart3_Send_Char;
	uart3.send_string = Ip_Uart3_Send_String;
}
