#include "IOStreamConsole.h"
#include <target.h>
#include <iostream>

static IOStreamConsole* debugConsole = nullptr;

Console* getConsole()
{
    if(!debugConsole)
    {
        debugConsole = new IOStreamConsole(std::cin, std::cout);
    }

    return debugConsole;
}

Pin* getSPIFlashCSPin()
{
	// what SPI flash CS pin?!
	return nullptr;
}

SPI* getSPIPeripheral()
{
	// not supported
	return nullptr;
}