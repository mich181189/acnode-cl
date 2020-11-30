set(HARDWARE_SRCS Spi.cpp
					destructors.cpp)

add_library(hardware ${HARDWARE_SRCS})
target_link_libraries(hardware FreeRTOS)