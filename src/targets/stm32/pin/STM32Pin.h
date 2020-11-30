#pragma once

#include <hardware/Pin.h>

// We need some definitions from the HAL
#ifdef STM32H7xx
#include "stm32h7xx_hal.h"
#endif

class STM32Pin : public Pin
{
public:
	STM32Pin(GPIO_TypeDef* port, uint32_t pin, PinDirection direction);
	~STM32Pin();

	void toggle() override;
	void on() override;
	void off() override;
	bool read() override;

private:
	GPIO_InitTypeDef settings;
	GPIO_TypeDef* port;
};
