#include "STM32Uart.h"
#include "core/CriticalSection.h"
#include "utilities/SemaphoreHolder.h"

#ifdef STM32H7xx
#include "stm32h7xx_hal.h"
#endif

static USART_TypeDef* num2uart(int uart)
{
    switch(uart)
    {
    case 0: // 0 -> debug port
        return USART3;
    default:
        return nullptr;
    };
}

static int uart2num(USART_TypeDef* u)
{
    if(u == USART3)
    {
        return 0;
    }


    return -1;
}

// for interrupt handlers to work, we need to stash the handles here
UART_HandleTypeDef uart_handles[1] = {0};

void STM32Uart::uart_task(void* uart_in)
{
    STM32Uart* uart = reinterpret_cast<STM32Uart*>(uart_in);
    unsigned char temp[256];
    for(;;)
    {
        { // lock scope
            SemaphoreHolder lock(uart->writeBufferMutex);
            taskENTER_CRITICAL();
            if(uart->writeBuffer.getFillLevel() > 0)
            {
                size_t num = uart->writeBuffer.read(temp, 256);
                HAL_UART_Transmit(&uart_handles[uart->uart], temp, num, 0xffffffff);
            }

            taskEXIT_CRITICAL();
        }
        taskYIELD();
    }
}

// called by the HAL when it needs the hardware configuring
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitTypeDef  GPIO_InitStruct;

        GPIO_InitStruct.Pin       = GPIO_PIN_8;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_9;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

// Called by the HAL when a port is closed
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        
    }
}

void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&uart_handles[uart2num(USART3)]);
}

STM32Uart::STM32Uart(int uart) :
    uart(uart),
    isOpen(false),
    tskHandle(nullptr),
    readBufferMutex(xSemaphoreCreateMutex()),
    writeBufferMutex(xSemaphoreCreateMutex()),
    readBuffer(1024),
    writeBuffer(1024)
{
    uart_handles[uart].Instance        = num2uart(uart);

    uart_handles[uart].Init.BaudRate   = 9600*2;
    uart_handles[uart].Init.WordLength = UART_WORDLENGTH_8B;
    uart_handles[uart].Init.StopBits   = UART_STOPBITS_1;
    uart_handles[uart].Init.Parity     = UART_PARITY_NONE;
    uart_handles[uart].Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    uart_handles[uart].Init.Mode       = UART_MODE_TX_RX;
    uart_handles[uart].AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    xTaskCreate(uart_task, "Uart Task", configMINIMAL_STACK_SIZE, this, tskIDLE_PRIORITY + 2, &tskHandle);
    vTaskSuspend(tskHandle);
}

STM32Uart::~STM32Uart()
{
    vTaskDelete(tskHandle);
    vSemaphoreDelete(readBufferMutex);
    vSemaphoreDelete(writeBufferMutex);
}

void STM32Uart::send(const char* data, size_t length)
{
    SemaphoreHolder lock(writeBufferMutex);
    writeBuffer.write(reinterpret_cast<const uint8_t*>(data), length);
}

size_t STM32Uart::receive(char* data, size_t max_length)
{
    SemaphoreHolder lock(readBufferMutex);
    return readBuffer.read(reinterpret_cast<uint8_t*>(data), max_length);
}

bool STM32Uart::open(int baud)
{
    CriticalSection sec;
    if(isOpen)
    {
        HAL_UART_DeInit(&uart_handles[uart]);
        vTaskSuspend(tskHandle);
        isOpen = false;
    }

    uart_handles[uart].Init.BaudRate = baud;

    if(HAL_UART_Init(&uart_handles[uart]) == HAL_OK)
    {
        isOpen = true;

        vTaskResume(tskHandle);
        return true;
    }


    return false;
}

void STM32Uart::close()
{
    CriticalSection sec;
    HAL_UART_DeInit(&uart_handles[uart]);
    vTaskSuspend(tskHandle);
    isOpen = false;
}