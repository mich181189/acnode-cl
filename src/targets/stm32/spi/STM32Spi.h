#pragma once

#include "hardware/Spi.h"

#ifdef STM32H7xx
#include "stm32h7xx_hal.h"
#endif

// For now, just assume there is one SPI bus. We can add support for 
// multiple instances later if needed.
class STM32Spi : public SPI
{
public:
	STM32Spi();
	~STM32Spi();

	void sendAndReceive(const uint8_t* txdata, uint8_t* rxdata, size_t length) override;

private:
	SPI_HandleTypeDef handle;
};