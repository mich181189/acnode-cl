Building for STM32
==================

# Prerequisites
On Fedora for example, install the following packages:
`sudo dnf install arm-none-eabi-newlib arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-binutils-cs`

# Build
1. Make sure you have checked out all submodules
1. Configure `cmake -DCMAKE_TOOLCHAIN_FILE=../targets/stm32/toolchains/STM32H743ZI.cmake ..`
1. Build `make -j`

# Notes
This port is loosely based around https://github.com/STMicroelectronics/STM32CubeH7/tree/master/Projects/NUCLEO-H743ZI/Applications/LwIP/LwIP_HTTP_Server_Netconn_RTOS
You can use `make flash` to deploy, but note that for the STM32H743 board you'll need a version of openocd newer than v0.10.0 - as of writing this means pulling from master