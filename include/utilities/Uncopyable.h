#pragma once

class Uncopyable
{
public:
    Uncopyable() {}
    virtual ~Uncopyable();
private:
    Uncopyable(Uncopyable&);
    Uncopyable& operator=(Uncopyable&);
};
