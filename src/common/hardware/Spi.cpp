#include "hardware/Spi.h"
#include "hardware/Pin.h"

SPI::SPI() :
	spiQueue(xQueueCreate(2, sizeof(SPIMessage))),
	spiMutex(xSemaphoreCreateMutex())
{

}

SPI::~SPI()
{
	vSemaphoreDelete(spiMutex);
	vQueueDelete(spiQueue);
}

// send or receive only
void SPI::send(const uint8_t* data, size_t length, Pin* chipSelect)
{
	lock();
	chipSelect->off();
	sendAndReceive(data, nullptr, length);
	chipSelect->on();
	unlock();
}

void SPI::receive(uint8_t* data, size_t length, Pin* chipSelect)
{
	lock();
	chipSelect->off();
	sendAndReceive(nullptr, data, length);
	chipSelect->on();
	unlock();
}

// Full duplex
void SPI::sendAndReceive(const uint8_t* txdata, uint8_t* rxdata, size_t length, Pin* chipSelect)
{
	lock();
	chipSelect->off();
	sendAndReceive(txdata, rxdata, length);
	chipSelect->on();
	unlock();
}

// Send some data then receive without raising CS
void SPI::sendThenReceive(const uint8_t* txdata, size_t txlength, uint8_t* rxdata, size_t rxlength, Pin* chipSelect)
{
	lock();
	chipSelect->off();
	sendAndReceive(txdata, nullptr, txlength);
	sendAndReceive(nullptr, rxdata, rxlength);
	chipSelect->on();
	unlock();
}

// Lower level functions - you are responsible for handling CS and making sure you have the lock
void SPI::lock()
{
	xSemaphoreTake(spiMutex, portMAX_DELAY);
}

void SPI::unlock()
{
	xSemaphoreGive(spiMutex);
}
