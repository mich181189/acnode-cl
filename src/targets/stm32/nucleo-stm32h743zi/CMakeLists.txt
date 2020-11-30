set(FREERTOS_EXTRA_FILES ${CMAKE_SOURCE_DIR}/targets/stm32/STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c)

add_library(freertos_extra ${FREERTOS_EXTRA_FILES})
target_link_libraries(freertos_extra FreeRTOS ${SYSTEM_LIBRARY})
target_include_directories(freertos_extra PUBLIC ${CMAKE_SOURCE_DIR}/targets/stm32/STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/)

set(TARGET_SRCS 
	interrupt_handlers.c
	../uart/STM32Uart.cpp
	../pin/STM32Pin.cpp
	../spi/STM32Spi.cpp
	../target.cpp
	)

add_library(target ${TARGET_SRCS})
target_link_libraries(target freertos_extra)