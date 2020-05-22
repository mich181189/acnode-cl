#include <core/ConsoleLogger.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "FreeRTOS.h"
#include "task.h"

const char* level2string(Logger::Level l)
{
    switch(l)
    {
        case Logger::LEVEL_TRACE:
            return "TRACE";
        case Logger::LEVEL_DEBUG:
            return "DEBUG";
        case Logger::LEVEL_INFO:
            return "INFO";
        case Logger::LEVEL_WARN:
            return "WARN";
        case Logger::LEVEL_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

const char* level2colour(Logger::Level l)
{
    switch(l)
    {
        case Logger::LEVEL_TRACE:
            return "\u001b[37m";
        case Logger::LEVEL_DEBUG:
            return "\u001b[34m";
        case Logger::LEVEL_INFO:
            return "\u001b[32m";
        case Logger::LEVEL_WARN:
            return "\u001b[31m";
        case Logger::LEVEL_ERROR:
            return "\u001b[31m";
        default:
            return "\u001b[0m";
    }
}

ConsoleLogger::ConsoleLogger(Console* c) :
    console(c)
{

}

ConsoleLogger::~ConsoleLogger()
{

}

bool ConsoleLogger::shouldLog(Level lvl)
{
    return true;
}

void ConsoleLogger::log(Level lvl, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char* buffer = (char*)malloc(1024);
    
    const char* taskname = pcTaskGetName(nullptr);
    sprintf(buffer, "%s[%s] \"%s\" %lu\u001b[0m  ", level2colour(lvl), level2string(lvl), taskname, xTaskGetTickCount());
    console->send(buffer, strnlen(buffer, 1024));    

    vsnprintf(buffer, 1024, fmt, args);
    console->send(buffer, strnlen(buffer, 1024));
    console->send("\r\n", 2);
    free(buffer);
}
