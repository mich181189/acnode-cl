#include <utilities/CircularBuffer.h>
#include <cstring>
#include <cmath>

CircularBuffer::CircularBuffer(size_t capacity) :
    buffer(static_cast<uint8_t*>(calloc(capacity, sizeof(uint8_t)))),
    capacity(capacity),
    fillLevel(0),
    readPoint(0),
    writePoint(0)
{

}

CircularBuffer::~CircularBuffer()
{
    free(buffer);
}

void CircularBuffer::write(const uint8_t* data, size_t size)
{
    // clamp to available space
    if(size > getRemaining())
    {
        size = getRemaining();
    }

    size_t part1 = std::min(capacity-writePoint, size);
    memcpy(buffer+writePoint, data, part1);
    writePoint += part1;

    // do we need to write more?
    // we've already checked it'll fit, so no need to 
    // check for clobbering readPoint here.
    if(part1 < size)
    {
        size_t part2 = size-part1;
        memcpy(buffer, data+part1, part2);
        writePoint = part2;
    }
    fillLevel += size;
}

size_t CircularBuffer::read(uint8_t* data, size_t size)
{
    size = std::min(size, getFillLevel());

    size_t part1 = std::min(size, capacity-readPoint);
    memcpy(data, buffer+readPoint, part1);
    readPoint += part1;

    if(part1 < size)
    {
        size_t part2 = size-part1;
        memcpy(data+part1, buffer, part2);
        readPoint = part2;
    }

    // handle readPoint wrap
    if(readPoint == capacity)
    {
        readPoint = 0;
    }

    fillLevel -= size;

    return size;
}
