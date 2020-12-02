#pragma once

#include <cstdint>

class File
{
public:
	virtual ~File();

	// Close this file. Pointer is no longer valid after this returns
	virtual void close()=0;

	// read up to size bytes into buffer. Returns actual count
	virtual uint32_t read(void* buffer, uint32_t size)=0;

	// write up to size bytes from buffer. Returns actual number written.
	// This will be the same as size, unless storage ran out
	virtual uint32_t write(const void* buffer, uint32_t size)=0;
};

class Filesystem
{
public:
	virtual ~Filesystem();

	virtual bool ready() const = 0;

	virtual File* open(const char* path)=0;

	virtual void mkdir(const char* path)=0;


};