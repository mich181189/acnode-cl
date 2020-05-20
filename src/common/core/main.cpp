#include "target.h"

#include "FreeRTOS.h"
#include "task.h"

#include "../../targets/stm32/uart/STM32Uart.h"

#include "stm32h7xx_hal.h"

void tempTask(void* nothing)
{
	STM32Uart uart(0);
	uart.open(115200);
	for(;;)
	{
		vTaskDelay(1000);
		uart.send("Hey!\n", 5);
	}
}

int main(int argc, char** argv)
{
    sys_init();
    register_interrupt_handlers();

    xTaskCreate(tempTask, "Test Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 3, nullptr);

    vTaskStartScheduler();
    while(1);
    return 0;
}
