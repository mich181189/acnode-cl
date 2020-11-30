
SET(STORAGE_SRCS SPIFlash.cpp)

set_source_files_properties(${STORAGE_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -Werror")

add_library(storage ${STORAGE_SRCS})
target_link_libraries(storage hardware)