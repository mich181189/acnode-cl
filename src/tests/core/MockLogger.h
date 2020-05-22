#pragma once

#include <gmock/gmock.h>

#include <core/Logger.h>

#include <cstdarg>
#include <cstring>

class MockLogger : public Logger
{
public:
    MOCK_METHOD(bool, shouldLog, (Logger::Level), (override));
    MOCK_METHOD(void , logResult, (Logger::Level, std::string msg));

    // mocking variadics doesn't really work, so we bake it down here
    void log(Level lvl, const char* fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);

        char buffer[1024];
        vsnprintf(buffer, 1024, fmt, args);
        va_end(args);

        logResult(lvl, std::string(buffer, strnlen(buffer, 1024)));
    }
};
