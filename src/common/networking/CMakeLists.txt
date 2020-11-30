
set(NETWORKING_SRCS )

if(${USE_LWIP})
list(APPEND NETWORKING_SRCS LWIPTasks.cpp)
endif()

set_source_files_properties(${NETWORKING_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -Werror")

add_library(networking ${NETWORKING_SRCS})

if(${USE_LWIP})
target_link_libraries(networking lwip lwip_ethernetif)
endif()
