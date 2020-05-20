#pragma once

#include <core/Uart.h>
#include <utilities/CircularBuffer.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// A wrapper around STM32's HAL UART
class STM32Uart : public Uart
{
public:
    STM32Uart(int uart);
    ~STM32Uart();

    void send(const char* data, size_t length) override;
    size_t receive(char* data, size_t max_length) override;

    bool open(int baud) override;
    void close() override;
private:
    static void uart_task(void* uart_in);

    int uart;

    bool isOpen;
    TaskHandle_t tskHandle;
    SemaphoreHandle_t readBufferMutex;
    SemaphoreHandle_t writeBufferMutex;
    CircularBuffer readBuffer;
    CircularBuffer writeBuffer;
};