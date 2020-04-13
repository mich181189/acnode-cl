
set(CORE_SRCS 
	main.cpp)

set_source_files_properties(${CORE_SRCS} PROPERTIES COMPILE_FLAGS -Wall)

add_executable(acnode-cl ${CORE_SRCS})
target_link_libraries(acnode-cl ${SYSTEM_LIBRARY})
makeFlash(acnode-cl)