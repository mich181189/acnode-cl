#include "target.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"

void register_interrupt_handlers()
{
	// this needs to exist, but doesn't need to contain anything
	// otherwise the linker assumes this file is useless
}

// The rest of these mostly come from STM32 examples

void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{
  osSystickHandler();
  HAL_IncTick();
}


extern ETH_HandleTypeDef EthHandle;

void ETH_IRQHandler(void)
{
  HAL_ETH_IRQHandler(&EthHandle);
}
