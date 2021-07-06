#include "misc.h"
#include "at32f4xx.h"
#include "at32f4xx_rcc.h"

/*标准库*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*freertos*/
#include "FreeRTOS.h"
#include "task.h"

/*app*/
#include "usart.h"
#include "gpio.h"

int main(void)
{
  __set_PRIMASK(0);
  SystemInit(); /*时钟初始化*/

  SCB->VTOR = FLASH_BASE | 0x00000;

  gpio_init();
  
  while (1)
  {
  }
}

void USART1_IRQHandler()
{
  if (USART_GetITStatus(USART1, USART_INT_IDLEF) != RESET)
  {
  }
}

void Extend_SRAM(void)
{
  // check if RAM has been set to 224K, if not, change EOPB0
  if (((UOPTB->EOPB0) & 0xFF) != 0xFE)
  {
    /* Unlock Option Bytes Program Erase controller */
    FLASH_Unlock();
    /* Erase Option Bytes */
    FLASH_EraseUserOptionBytes();
    /* Change SRAM size to 224KB */
    FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0, 0xFE);
    NVIC_SystemReset();
  }
}