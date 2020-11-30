
set(CORE_SRCS 
	main.cpp
	ConsoleLogger.cpp
	destructors.cpp
	Startup.cpp
	utils.cpp)

set_source_files_properties(${CORE_SRCS} PROPERTIES COMPILE_FLAGS -Wall)

add_executable(acnode-cl ${CORE_SRCS})
target_link_libraries(acnode-cl ${SYSTEM_LIBRARY} FreeRTOS target utilities networking hardware storage)
makeFlash(acnode-cl)