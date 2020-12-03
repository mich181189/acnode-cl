#include "Startup.h"
#include <core/ACNodeCore.h>
#include <core/ConsoleLogger.h>
#include <utilities/utils.h>
#include <target.h>
#include <FreeRTOS.h>
#include <task.h>

#ifdef USE_LWIP
#include <networking/LWIPTasks.h>
#endif

ACNodeCore core;

// Task to set up and initialise ACNode
void acnodeStatupTask(void*)
{
    core.logger = new ConsoleLogger(getConsole());

    LOG_INFO(core.logger, "Starting ACNode Rev " STRINGIFY(GIT_REVISION) );

    #ifdef USE_LWIP
    LOG_INFO(core.logger, "Starting LWIP" );
    initLWIP(&core);
    #endif

    core.filesystem = getFilesystem();

    // Startup complete - exit.
    LOG_INFO(core.logger, "Startup Complete");
    vTaskDelete(nullptr);
}