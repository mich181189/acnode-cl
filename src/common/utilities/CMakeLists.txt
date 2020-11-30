set(UTILITIES_SRCS CircularBuffer.cpp
					destructors.cpp)

set_source_files_properties(${UTILITIES_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -Werror")

add_library(utilities ${UTILITIES_SRCS})