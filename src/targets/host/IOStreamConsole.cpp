#include "IOStreamConsole.h"



IOStreamConsole::IOStreamConsole(std::istream &i, std::ostream &o) :
	i(i),
	o(o)
{

}

IOStreamConsole::~IOStreamConsole()
{

}

void IOStreamConsole::send(const char* data, size_t length)
{
	o.write(data, length);
}

size_t IOStreamConsole::receive(char* data, size_t max_length)
{
	return i.readsome(data, max_length);
}
