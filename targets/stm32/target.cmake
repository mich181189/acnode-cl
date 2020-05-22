###########################################################
# cmake include for STM32 targets
###########################################################

enable_language(ASM)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "${CMAKE_ASM_SOURCE_FILE_EXTENSIONS} .s")

if("${TARGET_NAME}" STREQUAL "STM32H743ZI")
set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/startup_stm32h743xx.s)
set(TARGET_SYS_FILES ${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/system_stm32h7xx.c
			  		${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/target_init.c)
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/target_init.c PROPERTIES COMPILE_FLAGS -Wall)
include(${CMAKE_CURRENT_LIST_DIR}/stm32cubeh7.cmake)

add_library(lwip_ethernetif ${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/lwip/ethernetif.c)
target_link_libraries(lwip_ethernetif lwip hal bsp cmsisos)
target_include_directories(lwip_ethernetif PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/lwip)

else()
message(FATAL_ERROR "Unknown STM32 target ${TARGET_NAME}")
endif()

set(FREERTOS_EXTRA_BITS )

add_library(stm32_core  ${TARGET_SYS_FILES} ${STARTUP_FILE})
target_link_libraries(stm32_core hal bsp)
add_library(stm32::core ALIAS stm32_core)