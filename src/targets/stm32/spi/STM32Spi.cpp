#include "STM32Spi.h"

#include <cstring>

//the HAL calls this when it wants us to set up the hardware
void HAL_SPI_MspInit(SPI_HandleTypeDef *spi)
{
	if(spi->Instance == SPI3)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_SPI3_CLK_ENABLE();

		// Set up some GPIO settings common to all
		GPIO_InitTypeDef gpioSettings;
		gpioSettings.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		gpioSettings.Mode = GPIO_MODE_AF_PP;
		gpioSettings.Pull = GPIO_PULLDOWN;

		//MISO - PB4
		gpioSettings.Pin = GPIO_PIN_4;
		gpioSettings.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOB, &gpioSettings);

		//MOSI - PB5
		gpioSettings.Pin = GPIO_PIN_5;
		gpioSettings.Alternate = GPIO_AF7_SPI3;
		HAL_GPIO_Init(GPIOB, &gpioSettings);

		// SCK - PB3
		gpioSettings.Pin = GPIO_PIN_3;
		gpioSettings.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOB, &gpioSettings);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spi)
{
	if(spi->Instance == SPI3)
	{
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
	}
}

STM32Spi::STM32Spi()
{
	memset(&handle, 0, sizeof(handle));
	handle.Instance = SPI3;
	handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	handle.Init.Mode = SPI_MODE_MASTER;
	handle.Init.Direction = SPI_DIRECTION_2LINES;
	handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	handle.Init.CRCPolynomial     = 7;
	handle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	handle.Init.NSS               = SPI_NSS_SOFT;
	handle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	handle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;

	HAL_SPI_Init(&handle);
}

STM32Spi::~STM32Spi()
{
	HAL_SPI_DeInit(&handle);
}

void STM32Spi::sendAndReceive(const uint8_t* txdata, uint8_t* rxdata, size_t length)
{
	if(txdata && rxdata)
	{
		//bidirectional
		HAL_SPI_TransmitReceive(&handle, const_cast<uint8_t*>(txdata), rxdata, length, 10000);
	}
	else if(txdata)
	{
		// send only
		HAL_SPI_Transmit(&handle, const_cast<uint8_t*>(txdata), length, 10000);
	}
	else
	{
		// receive only
		HAL_SPI_Receive(&handle, rxdata, length, 10000);
	}
}