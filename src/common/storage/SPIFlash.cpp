#include <storage/SPIFlash.h>
#include <hardware/Spi.h>
#include <hardware/Pin.h>
#include <utilities/SemaphoreHolder.h>
#include <core/ACNodeCore.h>

#include <semphr.h>
#include <task.h>

SPIFlash::SPIFlash(SPI* spi, Pin* select) :
	spi(spi),
	select(select),
	isready(false),
	flashmutex(xSemaphoreCreateMutex()),
	params(nullptr)
{
	LOG_INFO(core.logger, "SPI Flash Init");
	xTaskCreate(SPIFlash::FlashInterrogationTask, "SPI Flash Probe", configMINIMAL_STACK_SIZE*2, this, tskIDLE_PRIORITY+1, nullptr);
}

SPIFlash::~SPIFlash()
{
	vSemaphoreDelete(flashmutex);
}

// check if the storage is ready for use
bool SPIFlash::ready() const
{
	SemaphoreHolder holder(flashmutex);
	return isready;
}

void SPIFlash::readSFDP(uint32_t address, uint8_t* buffer, size_t length)
{
	uint8_t txscratch[5];
	txscratch[0] = 0x5a;
	txscratch[1] = ((address & 0xff0000) >> 16) & 0xff;
	txscratch[2] = ((address & 0xff00) >> 8) & 0xff;
	txscratch[3] = (address & 0xff);
	txscratch[4] = 0xff; // dummy byte

	spi->sendThenReceive(txscratch, 5, buffer, length, select);
}

void SPIFlash::FlashInterrogationTask(void* instance)
{
	LOG_INFO(core.logger, "Beginning SPI Flash Discovery");
	SPIFlash* sf = reinterpret_cast<SPIFlash*>(instance);

	// Firstly, we want to see if the flash supports JEDEC
	// Serial Flash Discoverable Parameters. If it does, we
	// can use those to learn everything we need, otherwise we
	// would need to fall back to a lookup table.

	// space for the JEDEC table we care about
	uint32_t data[9];

	//first read 4 bytes to see if we have a table
	sf->readSFDP(0x00, reinterpret_cast<uint8_t*>(data), 4);

	if(data[0] == 0x50444653)
	{
		// ok, now get the rest of the header
		sf->readSFDP(0x04, reinterpret_cast<uint8_t*>(data+1), 4);

		uint8_t majorVersion = (data[1] >> 8 & 0xff);
		uint8_t minorVersion = (data[1] & 0xff);

		LOG_INFO(core.logger, "Found SFDP V%hhu.%hhu", majorVersion, minorVersion);

		// read the header
		sf->readSFDP(0x08, reinterpret_cast<uint8_t*>(data+2), 8);
		if((data[2] & 0xff) == 0)
		{
			// now get the table version
			majorVersion = ((data[1] >> 16) & 0xff);
			minorVersion = ((data[1] >> 8) & 0xff);
			LOG_INFO(core.logger, "Found Parameter Header V%hhu.%hhu", majorVersion, minorVersion);
			
			uint8_t tableLength = ((data[2] >> 24) & 0xff);
			if(tableLength < 9)
			{
				LOG_WARN(core.logger, "Unexpected table length %hhu - expecting at least 9", tableLength);
			}
			// limit to the size we're expecting - there are some extra fields
			// we don't really care about specified in JESD216A (aka SFDP 1.5)
			if(tableLength > 9)
				tableLength = 9;

			sf->readSFDP(data[3] & 0xffffff, reinterpret_cast<uint8_t*>(data), 4*tableLength);

			sf->buildDescriptorsFromSFDP(data, tableLength);
		}
		else
		{
			LOG_ERROR(core.logger,"JEDEC table not found at first position - please implement a fallback! Got type %lx", data[2] & 0xff);
			sf->params = nullptr;
		}

	}
	else
	{
		LOG_ERROR(core.logger, "Flash does not support SFDP - please implement a lookup table! data is 0x%lx 0x%lx 0x%lx 0x%lx", (data[0] >> 24) & 0xff, (data[0] >> 16) & 0xff, (data[0] >> 8) & 0xff, data[0] & 0xff);
		sf->params = nullptr;
	}

	if(!sf->params)
	{
		LOG_ERROR(core.logger, "Unable to recognise flash memory")
		sf->params = {};
	}
	else
	{
		LOG_INFO(core.logger, "Found %lu bytes of SPI flash, can be erased in %lu byte sections using command 0x%hhx",
				sf->params->capacity, sf->params->sectorSize, sf->params->sectorEraseCommand);
		LOG_INFO(core.logger, "Flash is %s", sf->params->name);
		SemaphoreHolder holder(sf->flashmutex);
		sf->isready = true;
	}

	vTaskDelete(nullptr);
}

void SPIFlash::buildDescriptorsFromSFDP(uint32_t* data, size_t dwordCount)
{
	sfdpParams.name = "SFDP Discovered Flash";
	sfdpParams.jedecid = getJEDECID();

	// first parse size
	if(data[1] & 0x80000000)
	{
		// capacity is 2^data[1] bits - subtract 3 to get bytes
		int pow = (data[1] & 0x7fffffff) - 3;
		sfdpParams.capacity = 2 << pow;
	}
	else
	{
		// Capacity is directly specified in bits -> divide by 8 (shift by 3) to get bytes
		sfdpParams.capacity = (data[1] & 0x7fffffff) >> 3;
	}

	// do we have the higher tables? Very early implementations
	// supposedly only had 4
	if(dwordCount < 9)
	{
		LOG_INFO(core.logger, "SFDP table contains fewer than 9 fields - using legacy values");
		if((data[0] & 0x3) == 1)
		{
			// 4Kb mode supported
			sfdpParams.sectorSize = 4096*1024;
			sfdpParams.sectorEraseCommand = 0x20;
		}
		else
		{
			LOG_ERROR(core.logger, "4kb mode not supported - not enough information to proceed");
			sfdpParams = {};
			return;
		}
	}
	else
	{
		// Find the smallest block size we can erase
		// This is all LittleFS cares about
		uint32_t minSize = ~0;
		uint8_t minInstruction = 0;

		// There are two bytes for 4 possible sizes
		for(int i=7; i<9; ++i)
		{
			// upper word
			uint32_t size = ((data[i] >> 16) & 0xff);
			if(size != 0)
			{
				size = 1 << size;
				LOG_INFO(core.logger, "0x%lx can erase %lu bytes (raw 0x%lu)", (data[i] >> 24) & 0xff, size, ((data[i] >> 16) & 0xff));
				if(size < minSize)
				{
					minSize = size;
					minInstruction = (data[i] >> 24) & 0xff;
				}
			}

			// lower word
			size = (data[i] & 0xff);
			if(size != 0)
			{
				size = 1 << size;
				LOG_INFO(core.logger, "0x%lx can erase %lu bytes (raw 0x%lu)", (data[i] >> 8) & 0xff, size, (data[i]  & 0xff));
				if(size < minSize)
				{
					minSize = size;
					minInstruction = (data[i] >> 8) & 0xff;
				}
			}
		}

		sfdpParams.sectorEraseCommand = minInstruction;
		sfdpParams.sectorSize = minSize;

		// Set the SFDP params as the in-use parameters
		params = &sfdpParams;
	}
}

SPIFlashParams::JEDECID SPIFlash::getJEDECID()
{
	uint8_t readjedecidcmd = 0x9f;
	uint8_t jedecid[3];		
	spi->sendThenReceive(&readjedecidcmd, 1, jedecid, 3, select);
	return { jedecid[0], jedecid[1], jedecid[2] };
}