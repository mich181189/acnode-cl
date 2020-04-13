#include "target.h"

#include "stm32h7xx_nucleo.h"

int main(int argc, char** argv)
{
	sys_init();


	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

	/* Infinite loop */
	while (1)
	{
		BSP_LED_On(LED1);
		HAL_Delay(100);
		BSP_LED_Off(LED1);
		BSP_LED_On(LED2);
		HAL_Delay(100);
		BSP_LED_Off(LED2);
		BSP_LED_On(LED3);
		HAL_Delay(100);
		BSP_LED_Off(LED3);

	}

	return 0;
}
