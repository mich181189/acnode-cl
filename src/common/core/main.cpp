#include "target.h"
#include "Startup.h"

#include "FreeRTOS.h"
#include "task.h"

#include <core/ConsoleLogger.h>

#include <malloc.h>

int main(int argc, char** argv)
{
    target_init();
    register_interrupt_handlers();

    xTaskCreate(acnodeStatupTask, "Startup", configMINIMAL_STACK_SIZE*4, nullptr, tskIDLE_PRIORITY + 3, nullptr);

    vTaskStartScheduler();

    // FreeRTOS scheduler never returns
    while(1);
    return 0;
}
