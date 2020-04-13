#include "target.h"

#include "stm32h7xx_nucleo.h"

#include "FreeRTOS.h"
#include "task.h"

static void blinker( void *pvParameters )
{
	while(1)
	{
		BSP_LED_On(LED1);
		vTaskDelay(100);
		BSP_LED_Off(LED1);
		BSP_LED_On(LED2);
		vTaskDelay(100);
		BSP_LED_Off(LED2);
		BSP_LED_On(LED3);
		vTaskDelay(100);
		BSP_LED_Off(LED3);
	}
}


int main(int argc, char** argv)
{
	sys_init();
	register_interrupt_handlers();

	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_LED_On(LED1);
	BSP_LED_On(LED2);
	BSP_LED_On(LED3);

	xTaskCreate( blinker, "blinker", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );

	vTaskStartScheduler();

	return 0;
}
