include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

message(info "Building for ${TARGET_NAME}")

if("${TARGET_NAME}" STREQUAL "STM32H743ZI")
add_subdirectory(nucleo-stm32h743zi)
else()
message(fatal "Unknown target")
endif()