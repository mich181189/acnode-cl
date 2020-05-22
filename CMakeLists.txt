cmake_minimum_required (VERSION 3.1)
project (ACNode-CL)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(Git)

execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=40 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(-DGIT_REVISION=${GIT_SHA1})

include_directories(include)

set(USE_LWIP true)

if(DEFINED STM32)
	set(TARGET_PATH stm32)
elseif(NOT CMAKE_CROSSCOMPILING)
	enable_testing()
	set(TARGET_PATH host)

	# use LWIP on all non-host targets
	set(USE_LWIP false)
else()
	message(FATAL "Unknown target")
endif()

if(${USE_LWIP})
add_definitions(-DUSE_LWIP)
endif()

include_directories(src/targets/${TARGET_PATH}/include)

# Bring in the low level target support
add_subdirectory(targets)

# higher level target support
add_subdirectory(src/targets/${TARGET_PATH})

# slightly more generic stuff
add_subdirectory(external)
add_subdirectory(src)
