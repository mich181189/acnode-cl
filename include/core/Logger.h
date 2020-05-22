#pragma once

#include <string>
#include <sstream>

class Logger
{
public:
    enum Level
    {
        LEVEL_TRACE,
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARN,
        LEVEL_ERROR,
    };

    virtual ~Logger();

    virtual bool shouldLog(Level lvl) = 0;
    virtual void log(Level lvl, const char* fmt, ...) __attribute__((format(printf, 3, 4))) = 0;

};

#define LOG_EMIT(LOGGER, LVL, MSG, ...) \
    if((LOGGER)->shouldLog((LVL))) \
    {\
        (LOGGER)->log((LVL), MSG, ##__VA_ARGS__); \
    }

#define LOG_TRACE(LOGGER, MSG, ...) LOG_EMIT((LOGGER), Logger::LEVEL_TRACE, (MSG), ##__VA_ARGS__)
#define LOG_DEBUG(LOGGER, MSG, ...) LOG_EMIT((LOGGER), Logger::LEVEL_DEBUG, (MSG), ##__VA_ARGS__)
#define LOG_INFO(LOGGER, MSG, ...) LOG_EMIT((LOGGER), Logger::LEVEL_INFO, (MSG), ##__VA_ARGS__)
#define LOG_WARN(LOGGER, MSG, ...) LOG_EMIT((LOGGER), Logger::LEVEL_WARN, (MSG), ##__VA_ARGS__)
#define LOG_ERROR(LOGGER, MSG, ...) LOG_EMIT((LOGGER), Logger::LEVEL_ERROR, (MSG), ##__VA_ARGS__)
