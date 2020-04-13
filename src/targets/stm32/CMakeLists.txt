include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if("${TARGET_NAME}" STREQUAL "STM32H743ZI")
add_subdirectory(nucleo-stm32h743zi)
endif()