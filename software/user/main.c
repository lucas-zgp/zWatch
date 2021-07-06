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
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"

int main(void)
{
  unsigned char i,j;
	float t=0;

  __set_PRIMASK(0);
  SystemInit(); /*时钟初始化*/

  SCB->VTOR = FLASH_BASE | 0x00000;

  gpio_init();

  LCD_Init(); //LCD初始化
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

  while (1)
  {
    LCD_ShowString(0, 40, "LCD_W:", RED, WHITE, 16, 0);
    LCD_ShowIntNum(48, 40, LCD_W, 3, RED, WHITE, 16);
    LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
    LCD_ShowIntNum(128, 40, LCD_H, 3, RED, WHITE, 16);
    LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
    LCD_ShowString(0, 70, "Increaseing Nun:", RED, WHITE, 16, 0);
    LCD_ShowFloatNum1(128, 70, t, 4, RED, WHITE, 16);
    t += 0.11;
    for (j = 0; j < 3; j++)
    {
      for (i = 0; i < 6; i++)
      {
        LCD_ShowPicture(40 * i, 120 + j * 40, 40, 40, gImage_1);
      }
    }
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