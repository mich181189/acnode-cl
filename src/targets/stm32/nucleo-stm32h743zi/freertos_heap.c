#include <FreeRTOS.h>

__attribute__((section(".freertos_heap"))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];