#include <core/utils.h>

#include <core/Logger.h>

#include "FreeRTOS.h"
#include "task.h"

void logTaskStats(Logger* logger)
{
	LOG_DEBUG(logger, "Stack High Water Mark is %lu (lower means less available stack)", uxTaskGetStackHighWaterMark(nullptr));

	size_t taskcount = uxTaskGetNumberOfTasks();
	LOG_DEBUG(logger, "Task Count: %d", taskcount);

	TaskStatus_t* tasks = (TaskStatus_t*)malloc(sizeof(TaskStatus_t)*taskcount);

	taskcount = uxTaskGetSystemState(tasks, taskcount, nullptr);

	for(int i = 0; i < taskcount; ++i)
	{
		const char* state = "unknown";

		switch(tasks[i].eCurrentState)
		{
		case eReady:
			state = "Ready";
			break;
		case eRunning:
			state = "Running";
			break;
		case eBlocked:
			state = "Blocked";
			break;
		case eSuspended:
			state = "Suspended";
			break;
		case eDeleted:
			state = "Deleted";
			break;
		}

		LOG_DEBUG(logger, "%lu \"%s\" State: %s Priority: %lu High Water Mark: %d", tasks[i].xTaskNumber, tasks[i].pcTaskName, state, tasks[i].uxCurrentPriority, tasks[i].usStackHighWaterMark);
	}


	free(tasks);
}
