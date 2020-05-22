#include "target.h"
#include "Startup.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef STM32
#include "../../targets/stm32/uart/STM32Uart.h"
#else
#include "../../targets/host/IOStreamConsole.h"
#endif

#include <core/ConsoleLogger.h>

#include <malloc.h>

int main(int argc, char** argv)
{
    target_init();
    register_interrupt_handlers();

    xTaskCreate(acnodeStatupTask, "Startup", configMINIMAL_STACK_SIZE*4, nullptr, tskIDLE_PRIORITY + 3, nullptr);

    vTaskStartScheduler();
    while(1);
    return 0;
}
