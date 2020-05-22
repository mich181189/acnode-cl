#pragma once
#include <cstddef>

class Console
{
public:
    virtual ~Console();

    virtual void send(const char* data, size_t length)=0;
    virtual size_t receive(char* data, size_t max_length)=0;
};
