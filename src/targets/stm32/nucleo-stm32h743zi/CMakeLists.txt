
add_library(freertos_extra ALIAS cmsisos)

set(TARGET_SRCS 
	interrupt_handlers.c
	../uart/STM32Uart.cpp
	../pin/STM32Pin.cpp
	../spi/STM32Spi.cpp
	../target.cpp
	freertos_heap.c
	)

add_library(target ${TARGET_SRCS})
target_link_libraries(target freertos_extra storage)