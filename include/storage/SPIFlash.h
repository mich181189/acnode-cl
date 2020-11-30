#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <cstring>

class SPI;
class Pin;

struct SPIFlashParams
{
	// What do we call this thing?
	const char* name = "";

	struct JEDECID
	{
		uint8_t manufacturer = 0;
		uint8_t id1 = 0; // upper half
		uint8_t id2 = 0; // lower half

		bool operator==(const JEDECID& other) const 
		{
			return manufacturer == other.manufacturer && id1 == other.id1 && id2 == other.id2;
		}
	} jedecid;

	// How many bytes of data can this thing hold?
	uint32_t capacity = 0;

	// What is the smallest sector we can erase, and how do we do it?
	uint32_t sectorSize = 0;
	uint8_t sectorEraseCommand = 0;

	// Not currently supported by the software, but some options for
	// Winbond's multi-die ICs should we ever want to use them
	// These behave like multiple ICs attached to the same SPI bus,
	// with a command to switch between them
	uint8_t diecount = 1;
	uint8_t dieswitchcommand = 0;

	bool operator==(const SPIFlashParams& other) const
	{
		return
			(strcmp(name, other.name) == 0) &&
			jedecid == other.jedecid &&
			capacity == other.capacity &&
			sectorSize == other.sectorSize &&
			diecount == other.diecount &&
			dieswitchcommand == other.dieswitchcommand;
	}

	SPIFlashParams& operator=(const SPIFlashParams& other)
	{
		// since this is just a bunch of static data, just memcpy it
		memcpy(this, &other, sizeof(SPIFlashParams));

		return *this;
	}
};

// SPIFlash provides access to a SPI flash chip.
class SPIFlash
{
public:
	SPIFlash(SPI* spi, Pin* select);
	~SPIFlash();

	// check if the storage is ready for use
	bool ready() const;

	SPIFlashParams::JEDECID getJEDECID();

private:
	static void FlashInterrogationTask(void*);

	void readSFDP(uint32_t address, uint8_t* buffer, size_t length);

	void buildDescriptorsFromSFDP(uint32_t* data, size_t dwordCount);

	SPI* spi;
	Pin* select;

	bool isready;

	// to protect the ready variable and other state
	SemaphoreHandle_t flashmutex;

	// Data about connected flash

	// scratch space for params discovered from SFDP
	SPIFlashParams sfdpParams;
	// Pointer to actual, in use params
	SPIFlashParams *params;
};
