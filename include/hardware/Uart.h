#pragma once

#include <core/Console.h>

class Uart : public Console
{
public:
    virtual bool open(int baud)=0;
    virtual void close()=0;
};
