
set(FreeRTOS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/event_groups.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/list.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/queue.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/tasks.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/timers.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/stream_buffer.c
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)

if(DEFINED STM32)
list(APPEND FreeRTOS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
)
set(PORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F)
else()

# linux

list(APPEND FreeRTOS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS_Posix/FreeRTOS_Kernel/portable/GCC/Posix/port.c)
set(PORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS_Posix/FreeRTOS_Kernel/portable/GCC/Posix/)

endif()

add_library(FreeRTOS ${FreeRTOS_SRCS})

if(DEFINED STM32)
target_link_libraries(FreeRTOS hal)

else()
find_package(Threads REQUIRED)
target_link_libraries(FreeRTOS Threads::Threads)

endif()

target_include_directories(FreeRTOS SYSTEM
	PUBLIC ${PORT_HEADERS}
			${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/include)