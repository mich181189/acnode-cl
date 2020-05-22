#include <core/utils.h>

#include <core/Logger.h>

#include "FreeRTOS.h"
#include "task.h"

void logTaskStats(Logger* logger)
{
	LOG_DEBUG(logger, "Stack High Water Mark is %lu (lower means less available stack)", uxTaskGetStackHighWaterMark(nullptr))
}
