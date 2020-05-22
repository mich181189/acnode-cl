#pragma once
#include "Logger.h"
#include "Console.h"
#include <utilities/Uncopyable.h>

class ConsoleLogger :   public Logger,
                        public Uncopyable
{
public:
    ConsoleLogger(Console* c);
    ~ConsoleLogger();

    bool shouldLog(Level lvl) override;
    void log(Level lvl, const char* fmt, ...) override;
private:
    Console* console;
};