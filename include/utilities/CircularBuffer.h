#pragma once

#include <cstdint>
#include <cstdlib>

class CircularBuffer
{
public:
    CircularBuffer(size_t capacity);
    ~CircularBuffer();

    void write(const uint8_t* data, size_t size);
    size_t read(uint8_t* data, size_t size);

    bool full() const { return getRemaining() == 0; }
    bool empty() const { return getFillLevel() == 0; }

    size_t getCapacity() const { return capacity; }
    size_t getRemaining() const { return capacity-getFillLevel(); }
    size_t getFillLevel() const { return fillLevel; }

private:
    uint8_t* buffer;
    size_t capacity;
    size_t fillLevel;

    size_t readPoint;
    size_t writePoint;
};
