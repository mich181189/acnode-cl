
SET(STORAGE_SRCS SPIFlash.cpp)

add_library(storage ${STORAGE_SRCS})
target_link_libraries(storage hardware)