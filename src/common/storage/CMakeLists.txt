
SET(STORAGE_SRCS
	LittleFS.cpp
	SPIFlash.cpp
	destructors.cpp)

set_source_files_properties(${STORAGE_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -Werror")

add_library(storage ${STORAGE_SRCS})
target_link_libraries(storage hardware littlefs)