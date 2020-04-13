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

add_subdirectory(targets)

if(DEFINED STM32)
include_directories(src/targets/stm32/include)
add_subdirectory(src/targets/stm32)
endif()

add_subdirectory(external)
add_subdirectory(src)
