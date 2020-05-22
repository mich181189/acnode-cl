#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

class SemaphoreHolder
{
public:
    SemaphoreHolder(SemaphoreHandle_t s, TickType_t timeToWait=portMAX_DELAY) : s(s)
    {
        xSemaphoreTake(s, timeToWait);
    }

    SemaphoreHolder() : s(nullptr)
    {

    }

    SemaphoreHolder(SemaphoreHolder&& other) : s(other.s)
    {
        other.s = nullptr;
    }

    SemaphoreHolder& operator=(SemaphoreHolder&& other)
    {
        if(s)
        {
            xSemaphoreGive(s);
        }

        s = other.s;
        other.s = nullptr;

        return *this;
    }

    ~SemaphoreHolder()
    {
        if(s)
        {
            xSemaphoreGive(s);
        }
    }

private:
    SemaphoreHandle_t s;
    SemaphoreHolder(SemaphoreHolder& other);
    SemaphoreHolder& operator=(SemaphoreHandle_t& other);
};
