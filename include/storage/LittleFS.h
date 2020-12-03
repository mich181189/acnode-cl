#pragma once

#include "Filesystem.h"

#include <lfs.h>
#include <FreeRTOS.h>
#include "semphr.h"

#include <cstdint>

class SPIFlash;

class LittleFS : public Filesystem
{
public:
	LittleFS(SPIFlash* flash);
	~LittleFS();

	bool ready() const override;
	bool errored() const override;

	File* open(const char* path) override;

	void mkdir(const char* path) override;

	// used by LittleFSFile implementation detail
	void lock();
	void unlock();

private:
	static void startupTask(void* fs);

	// LFS callbacks
	static int lfs_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size);
	static int lfs_prog(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size);
	static int lfs_erase(const struct lfs_config *c, lfs_block_t block);
	static int lfs_sync(const struct lfs_config *c);

	SPIFlash* flash;


	SemaphoreHandle_t fsmutex;

	bool isready;
	bool hasError;

	lfs_t lfs;
	struct lfs_config cfg;
};