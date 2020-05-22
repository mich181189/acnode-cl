#pragma once

#include "FreeRTOS.h"
#include "task.h"

class CriticalSection
{
public:
	CriticalSection()
	{
		taskENTER_CRITICAL();
	}

	~CriticalSection()
	{
		taskEXIT_CRITICAL();
	}

private:
	// No copying...
	CriticalSection(CriticalSection& other);
	CriticalSection& operator=(CriticalSection& other); 
};
