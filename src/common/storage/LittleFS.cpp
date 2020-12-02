#include <core/ACNodeCore.h>
#include <storage/LittleFS.h>
#include <storage/SPIFlash.h>

#include "task.h"
#include <cstring>

class LittleFSFile : public File
{
public:
	LittleFSFile(LittleFS* fs, lfs_t* lfs);
	~LittleFSFile();

	void close() override;

	// read up to size bytes into buffer. Returns actual count
	uint32_t read(void* buffer, uint32_t size) override;

	// write up to size bytes from buffer. Returns actual number written.
	// This will be the same as size, unless storage ran out
	uint32_t write(const void* buffer, uint32_t size) override;

	// used by LittleFS class to open file
	lfs_file_t* getFile() { return &file; }

private:
	LittleFS* fs;
	lfs_t* lfs;
	lfs_file_t file;
};

LittleFSFile::LittleFSFile(LittleFS* fs, lfs_t* lfs) :
	fs(fs),
	lfs(lfs)
{

}

LittleFSFile::~LittleFSFile()
{

}

void LittleFSFile::close()
{
	delete this;
}

uint32_t LittleFSFile::read(void* buffer, uint32_t size)
{
	return 0;
}

uint32_t LittleFSFile::write(const void* buffer, uint32_t size)
{
	return 0;
}

LittleFS::LittleFS(SPIFlash* flash) :
	flash(flash),
	isready(false)
{
	memset(&lfs, 0, sizeof(lfs));
	memset(&cfg, 0, sizeof(cfg));

	xTaskCreate(LittleFS::startupTask, "LittleFS Init", configMINIMAL_STACK_SIZE*2, this, tskIDLE_PRIORITY+1, nullptr);
}

LittleFS::~LittleFS()
{

}

bool LittleFS::ready() const
{
	return isready;
}

File* LittleFS::open(const char* path)
{
	return nullptr;
}

void LittleFS::mkdir(const char* path)
{

}

void LittleFS::startupTask(void* in)
{
	LittleFS* fs = reinterpret_cast<LittleFS*>(in);

	// wait for flash to be ready
	while(!fs->flash->ready())
	{
		vTaskDelay(100);
	}

	// if the flash has errored, there's no point continuing
	if(fs->flash->errored())
	{
		LOG_ERROR(core.logger, "Flash storage error - no point continuing");
		vTaskDelete(nullptr);
		return;
	}

	const SPIFlashParams* flashParams = fs->flash->getParams();

	// callback stuff
	fs->cfg.context = fs;
	fs->cfg.read = LittleFS::lfs_read;
	fs->cfg.prog = LittleFS::lfs_prog;
	fs->cfg.erase = LittleFS::lfs_erase;
	fs->cfg.sync = LittleFS::lfs_sync;

	// TODO: Check performance to make sure this doesn't cause loads of stupidly small reads
	fs->cfg.read_size = 1;
	fs->cfg.prog_size = 1;

	fs->cfg.block_size = flashParams->sectorSize;
	fs->cfg.block_count = flashParams->capacity/flashParams->sectorSize;
	fs->cfg.block_cycles = 200;

	// Cache one page - we're not super short on memory here
	fs->cfg.cache_size = flashParams->pageSize;

	fs->cfg.lookahead_size = 16;

	int retval = lfs_mount(&fs->lfs, &fs->cfg);
	// check to see if we got LFS_ERR_CORRUPT - this probably
	// just means we need to initialise the filesystem
	if(retval == LFS_ERR_CORRUPT)
	{
		LOG_WARN(core.logger, "Filesystem not valid - formatting");
		retval = lfs_format(&fs->lfs, &fs->cfg);
		if(retval != LFS_ERR_OK)
		{
			LOG_ERROR(core.logger, "Error formatting - %d", retval);
		}
		else
		{
			LOG_INFO(core.logger, "Formatting complete. Re-attempting mount");
			retval = lfs_mount(&fs->lfs, &fs->cfg);
		}
	}

	if(retval != LFS_ERR_OK)
	{
		LOG_ERROR(core.logger, "Error mounting LittleFS filesystem - %d", retval);
	}
	else
	{
		LOG_INFO(core.logger, "Mounted LittleFS filesystem");
	}

	vTaskDelete(nullptr);
}

int LittleFS::lfs_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size)
{
	LittleFS* fs = reinterpret_cast<LittleFS*>(c->context);

	fs->flash->read(block*c->block_size + off, size, reinterpret_cast<uint8_t*>(buffer));

	return LFS_ERR_OK;
}

int LittleFS::lfs_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size)
{
	LittleFS* fs = reinterpret_cast<LittleFS*>(c->context);

	fs->flash->write(block*c->block_size + off, size, reinterpret_cast<const uint8_t*>(buffer));

	return LFS_ERR_OK;
}

int LittleFS::lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
	LittleFS* fs = reinterpret_cast<LittleFS*>(c->context);

	fs->flash->eraseBlock(block*c->block_size);
	return LFS_ERR_OK;
}

int LittleFS::lfs_sync(const struct lfs_config *c)
{
	return LFS_ERR_OK;
}
