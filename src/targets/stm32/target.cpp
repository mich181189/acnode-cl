#include <target.h>
#include "uart/STM32Uart.h"

static STM32Uart* debugUART = nullptr;

Console* getConsole()
{
	if(!debugUART)
	{
		debugUART = new STM32Uart(0);
		debugUART->open(115200);
	}

	return debugUART;
}