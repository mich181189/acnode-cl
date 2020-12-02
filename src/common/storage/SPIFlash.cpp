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
	iserror(false),
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

bool SPIFlash::errored() const
{
	SemaphoreHolder holder(flashmutex);
	return iserror;
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

	// space for the JEDEC table
	uint32_t data[16];

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

			// just in case some future device has more tables
			if(tableLength > 16)
				tableLength = 16;

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
		sf->iserror = true;
	}
	else
	{
		LOG_INFO(core.logger, "Found %lu bytes of SPI flash, can be erased in %lu byte sections using command 0x%hhx",
				sf->params->capacity, sf->params->sectorSize, sf->params->sectorEraseCommand);
		LOG_INFO(core.logger, "Flash can be written in (at least) %lu byte pages", sf->params->pageSize);
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


	// do we have the higher dwords?
	if(dwordCount >= 11)
	{
		uint8_t n = uint8_t((data[10] >> 4) & 0xf);
		sfdpParams.pageSize = 1 << n;
	}
	else
	{
		if((data[0] & 0x4) == 0x4)
		{
			// we know the flash supports at least this.
			// It might support more, but we can't be sure
			sfdpParams.pageSize = 64;
		}
		else
		{
			// Oh dear. This will be really slow.
			sfdpParams.pageSize = 1;
			LOG_WARN(core.logger, "Device reports it does not support 64Kb page writes");
			LOG_WARN(core.logger, "This means we must assume we can only write one byte at a time!");
			LOG_WARN(core.logger, "This will be slow!");
		}
	}

	// do we have the higher tables? Very early implementations
	// supposedly only had 4
	if(dwordCount < 9)
	{
		LOG_INFO(core.logger, "SFDP table contains fewer than 9 fields - using legacy values");
		if((data[0] & 0x3) == 1)
		{
			// 4Kb mode supported
			sfdpParams.sectorSize = 4096;
			sfdpParams.sectorEraseCommand = (data[0] >> 8) & 0xff;
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
				LOG_INFO(core.logger, "Command 0x%lx can erase %lu bytes (raw 0x%lu)", (data[i] >> 24) & 0xff, size, ((data[i] >> 16) & 0xff));
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
				LOG_INFO(core.logger, "Command 0x%lx can erase %lu bytes (raw 0x%lu)", (data[i] >> 8) & 0xff, size, (data[i]  & 0xff));
				if(size < minSize)
				{
					minSize = size;
					minInstruction = (data[i] >> 8) & 0xff;
				}
			}
		}

		sfdpParams.sectorEraseCommand = minInstruction;
		sfdpParams.sectorSize = minSize;
	}
	// Set the SFDP params as the in-use parameters
	params = &sfdpParams;
}

SPIFlashParams::JEDECID SPIFlash::getJEDECID()
{
	uint8_t readjedecidcmd = 0x9f;
	uint8_t jedecid[3];		
	spi->sendThenReceive(&readjedecidcmd, 1, jedecid, 3, select);
	return { jedecid[0], jedecid[1], jedecid[2] };
}

bool SPIFlash::busy() const
{
	uint8_t command = 0x05;
	uint8_t response;
	spi->sendThenReceive(&command, 1, &response, 1, select);
	return (response & 0x1);
}

void SPIFlash::eraseBlock(uint32_t address)
{
	waitForIdle();
	enableWrites();

	uint8_t command[4] = {
		params->sectorEraseCommand,
		uint8_t((address >> 16) & 0xff),
		uint8_t((address >> 8) & 0xff),
		uint8_t((address) & 0xff),
		};
	spi->send(command, 4, select);
}

void SPIFlash::waitForIdle() const
{
	while(busy())
	{
		vTaskDelay(1);
	}
}

void SPIFlash::enableWrites()
{
	uint8_t command = 0x06;
	spi->send(&command, 1, select);
}

void SPIFlash::disableWrites()
{
	uint8_t command = 0x04;
	spi->send(&command, 1, select);
}

void SPIFlash::eraseChip()
{
	waitForIdle();
	enableWrites();
	uint8_t command = 0xc7;
	spi->send(&command, 1, select);
}

void SPIFlash::read(uint32_t address, uint32_t count, uint8_t* buffer)
{
	waitForIdle();

	uint8_t command[4] = {
		0x03, // READ command
		static_cast<uint8_t>((address >> 16) & 0xff),
		static_cast<uint8_t>((address >> 8) & 0xff),
		static_cast<uint8_t>((address) & 0xff),
		};
	spi->sendThenReceive(command, 4, buffer, count, select);
}

void SPIFlash::write(uint32_t address, uint32_t count, const uint8_t* buffer)
{
	uint32_t progress = 0;

	while(progress < count)
	{
		// We can write at most pageSize bytes
		uint32_t toWrite = std::min(count-progress, params->pageSize);
		
		// calculate the address we write to this time
		uint32_t dest = address + progress;
		
		//we can only write to the next page boundary
		uint32_t pageOffset = dest % params->pageSize;
		if((toWrite + pageOffset) > params->pageSize)
		{
			toWrite -= pageOffset;
		}

		writeInternal(dest, toWrite, buffer+progress);
		progress += toWrite;
	}
}

void SPIFlash::writeInternal(uint32_t address, uint32_t count, const uint8_t* buffer)
{
	waitForIdle();
	enableWrites();

	uint8_t command[4] = {
		0x02, // WRITE command
		static_cast<uint8_t>((address >> 16) & 0xff),
		static_cast<uint8_t>((address >> 8) & 0xff),
		static_cast<uint8_t>((address) & 0xff),
		};

	// we take lower level control here so we can
	// avoid a copy of the data
	spi->lock();
	select->off();
	spi->sendAndReceive(command, nullptr, 4);
	spi->sendAndReceive(buffer, nullptr, count);
	select->on();
	spi->unlock();
}
