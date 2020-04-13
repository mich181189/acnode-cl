
add_subdirectory(common)

if(NOT CMAKE_CROSSCOMPILING)
	enable_testing()
	add_subdirectory(tests)
endif()