
add_subdirectory(common)

if(NOT CMAKE_CROSSCOMPILING)
	add_subdirectory(tests)
endif()