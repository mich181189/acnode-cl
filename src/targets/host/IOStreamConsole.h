#include <core/Console.h>
#include <iostream>

class IOStreamConsole : public Console
{
public:
	IOStreamConsole(std::istream &i, std::ostream &o);
	~IOStreamConsole();

	void send(const char* data, size_t length) override;
    size_t receive(char* data, size_t max_length) override;

private:
	std::istream &i;
	std::ostream &o;
};
