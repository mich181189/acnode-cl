
function(AddACNodeTest)
	set(options ALWAYS)
	set(multiValueArgs FILES DEPENDENCIES)
	set(oneValueArgs TESTNAME)
	cmake_parse_arguments(AddACNodeTest "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	add_executable(${AddACNodeTest_TESTNAME}_Test ${AddACNodeTest_FILES})
	target_link_libraries(${AddACNodeTest_TESTNAME}_Test ${AddACNodeTest_DEPENDENCIES} googlemockmain)
	add_test(${AddACNodeTest_TESTNAME} ${AddACNodeTest_TESTNAME}_Test)
endfunction()

add_subdirectory(utilities)
add_subdirectory(core)