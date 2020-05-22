
set(NETWORKING_SRCS )

if(${USE_LWIP})
list(APPEND NETWORKING_SRCS LWIPTasks.cpp)
endif()


add_library(networking ${NETWORKING_SRCS})

if(${USE_LWIP})
target_link_libraries(networking lwip lwip_ethernetif)
endif()
