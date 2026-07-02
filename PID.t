#include "App_Layer.h"

int main()
{
	App_Delay_Init();
	App_Uart_Init(USART1, 115200);
	
	App_Logic_Init();
	for (;;)
	{
		App_Logic_Run();
	}
}
