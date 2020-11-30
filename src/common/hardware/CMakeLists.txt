set(HARDWARE_SRCS Spi.cpp
					destructors.cpp)

set_source_files_properties(${HARDWARE_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -Werror")

add_library(hardware ${HARDWARE_SRCS})
target_link_libraries(hardware FreeRTOS)