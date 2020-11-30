#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include <cstdint>

// Forward Declarations
class Pin;

// Messages that may appear on the SPI queue
enum SPIMessage
{
	MSG_DONE,
	MSG_ERROR	
};

class SPI
{
public:
	SPI();
	virtual ~SPI();

	// send or receive only
	void send(const uint8_t* data, size_t length, Pin* chipSelect);
	void receive(uint8_t* data, size_t length, Pin* chipSelect);

	// Full duplex
	void sendAndReceive(const uint8_t* txdata, uint8_t* rxdata, size_t length, Pin* chipSelect);

	// Send some data then receive without raising CS
	void sendThenReceive(const uint8_t* txdata, size_t txlength, uint8_t* rxdata, size_t rxlength, Pin* chipSelect);

	// Lower level functions - you are responsible for handling CS and making sure you have the lock
	void lock();
	void unlock();
	virtual void sendAndReceive(const uint8_t* txdata, uint8_t* rxdata, size_t length)=0;

protected:
	QueueHandle_t spiQueue;

private:
	SemaphoreHandle_t spiMutex;
};