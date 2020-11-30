#include "STM32Pin.h"

STM32Pin::STM32Pin(GPIO_TypeDef* port, uint32_t pin, PinDirection direction) :
	port(port)
{
	settings.Pin = pin;
	if(direction = PIN_DIRECTION_INPUT)
	{
		settings.Mode = GPIO_MODE_INPUT;
		settings.Pull = GPIO_PULLUP;
	}
	else
	{
		settings.Mode = GPIO_MODE_OUTPUT_PP;
		settings.Pull = GPIO_NOPULL;
	}
	
	
	settings.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	if(port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if(port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if(port == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if(port == GPIOH)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
	else if(port == GPIOI)
	{
		__HAL_RCC_GPIOI_CLK_ENABLE();
	}
	else if(port == GPIOJ)
	{
		__HAL_RCC_GPIOJ_CLK_ENABLE();
	}
	else if(port == GPIOK)
	{
		__HAL_RCC_GPIOK_CLK_ENABLE();
	}


	HAL_GPIO_Init(port, &settings);
	off();
}

STM32Pin::~STM32Pin()
{
	HAL_GPIO_DeInit(port, settings.Pin);
}

void STM32Pin::toggle()
{
	HAL_GPIO_TogglePin(port, settings.Pin);
}

void STM32Pin::on()
{
	HAL_GPIO_WritePin(port, settings.Pin, GPIO_PIN_SET);
}

void STM32Pin::off()
{
	HAL_GPIO_WritePin(port, settings.Pin, GPIO_PIN_RESET);
}

bool STM32Pin::read()
{
	return HAL_GPIO_ReadPin(port, settings.Pin) == GPIO_PIN_SET;
} 
