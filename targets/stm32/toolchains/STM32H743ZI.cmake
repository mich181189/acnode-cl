###########################################################
# cmake toolchain for STM32H743ZI Nucleo board
###########################################################

set(CMAKE_SYSTEM_NAME stm32)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/..)

# Ideally this would be in the binary directory, however
# when it runs tests it makes a mess of that
set(TOOLCHAIN_DIR ${PROJECT_SOURCE_DIR}/../toolchain/Connected_Launchpad)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../stm32h743zi/STM32H743ZITx_FLASH.ld)

# TODO: Tune these - they're Launchpad ones butchered
set(CMAKE_C_FLAGS "-g -Os -ffunction-sections -fdata-sections -ffreestanding --param max-inline-insns-single=500 -MMD -mthumb -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -specs=nosys.specs " CACHE STRING "")

set(CMAKE_CXX_FLAGS "-g -Os -ffunction-sections -fdata-sections -ffreestanding --param max-inline-insns-single=500 -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics  -MMD -mthumb -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nosys.specs "  CACHE STRING "")


set(TARGET_NAME STM32H743ZI)

add_definitions(
                    -DBOARD_NAME=NUCLEO_H743ZI
                    -DSTM32H7xx 
                    -DSTM32H743xx
                    -DUSE_HAL_DRIVER
                    -DUSE_STM32H7XX_NUCLEO_144_MB1364
                )

set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fsingle-precision-constant -ffreestanding -flto -T ${LINKER_SCRIPT}   -Xlinker -Map=output.map  -Wl,--entry=Reset_Handler " CACHE STRING "")

set(STLINK_CONFIG board/st_nucleo_h743zi.cfg)