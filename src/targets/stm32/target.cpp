#include <target.h>
#include "uart/STM32Uart.h"
#include "pin/STM32Pin.h"
#include "spi/STM32Spi.h"

#include <storage/SPIFlash.h>

static STM32Uart* debugUART = nullptr;

static STM32Pin* SPIFlashCSPin = nullptr;

static STM32Spi* spi = nullptr;

Console* getConsole()
{
	if(!debugUART)
	{
		debugUART = new STM32Uart(0);
		debugUART->open(115200);
	}

	return debugUART;
}

Pin* getSPIFlashCSPin()
{
	if(!SPIFlashCSPin)
	{
		SPIFlashCSPin = new STM32Pin(GPIOA, GPIO_PIN_4, PIN_DIRECTION_OUTPUT);
		SPIFlashCSPin->on();
	}

	return SPIFlashCSPin;
}

SPI* getSPIPeripheral()
{
	if(!spi)
	{
		spi = new STM32Spi();
	}

	return spi;
}

void beginStorageInit()
{
	SPIFlash* sf = new SPIFlash(getSPIPeripheral(), getSPIFlashCSPin());
}