set(CMAKE_EXECUTABLE_SUFFIX .elf)

set(STM32 1)

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "${CMAKE_ASM_SOURCE_FILE_EXTENSIONS} .S")

add_definitions(-DSTM32)

set(SYSTEM_LIBRARY stm32::core)

function(makeFlash TARGET)
    add_custom_target(${TARGET}.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> $<TARGET_FILE:${TARGET}>.bin
        DEPENDS ${TARGET})
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload-commands.cfg 
            CONTENT "init
reset init
halt
flash write_image erase $<TARGET_FILE:${TARGET}>.bin 0x08000000
reset
shutdown
")
    add_custom_target(flash 
        COMMAND openocd -f ${STLINK_CONFIG} -f ${CMAKE_CURRENT_BINARY_DIR}/upload-commands.cfg
        DEPENDS ${TARGET}.bin
        )
endfunction(makeFlash)