#pragma once

enum PinDirection
{
	PIN_DIRECTION_INPUT,
	PIN_DIRECTION_OUTPUT,
};

// Interface to a pin - can be turned on or off, or be read as an input
class Pin
{
public:
	virtual ~Pin();

	// output functions
	virtual void toggle()=0;
	virtual void on()=0;
	virtual void off()=0;

	// input functions
	virtual bool read()=0;
};