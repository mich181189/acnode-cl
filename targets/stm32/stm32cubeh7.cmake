###########################################################
# cmake include for building the HAL and a few bits of CMSIS for STM32H7 platforms
###########################################################

set(HAL_FILES 
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dsi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dts.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hrtim.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_msp_template.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_otfdec.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pssi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart_ex.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_wwdg.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_adc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_bdma.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_comp.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_crc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_crs.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dac.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dma2d.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dma.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_exti.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_gpio.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_hrtim.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_lptim.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_lpuart.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_mdma.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_opamp.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_pwr.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rcc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rng.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rtc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_spi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_swpmi.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_tim.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usart.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
	${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c

	${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/stm32h7xx_hal_timebase_tim.c
	)



add_library(hal ${HAL_FILES})
target_include_directories(hal 
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Inc
			${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/CMSIS/Device/ST/STM32H7xx/Include
			${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/CMSIS/Core/Include
			${CMAKE_CURRENT_LIST_DIR}/stm32h743zi/include)

if("${TARGET_NAME}" STREQUAL "STM32H743ZI")
	add_library(bsp ${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.c
					${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/BSP/Components/lan8742/lan8742.c)
	target_link_libraries(bsp hal)
	target_include_directories(bsp
		PUBLIC ${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Drivers/BSP/STM32H7xx_Nucleo)
endif()

add_library(cmsisos ${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c)
target_include_directories(cmsisos PUBLIC ${CMAKE_CURRENT_LIST_DIR}/STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/)
target_link_libraries(cmsisos FreeRTOS)