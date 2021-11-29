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
#include "bm8563.h"
#include "delay.h"
#include "timer.h"

/*lvgl*/
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_apps\demo\demo.h"
#include "lv_tests\lv_test_theme\lv_test_theme_1.h"
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"

#define TEST_NUM 3 //1,2,3 分别对应三个演示例程

int main(void)
{

  unsigned char i, j;
  float t = 0;

  __set_PRIMASK(0);
  SystemInit(); /*时钟初始化*/

  SCB->VTOR = FLASH_BASE | 0x00000;

  usart1_proxy_init();
  gpio_init();
  timer6_init();

  spi_init();
  LCD_Init(); //LCD初始化

  // LCD_Fill(0, 0, LCD_W, LCD_H, GRAYBLUE);

  lv_init();           /*lvgl 系统初始化*/
  lv_port_disp_init(); /*lvgl 显示接口初始化,放在 lv_init()的后面*/

  // Set_Start_BM8563();

#if (TEST_NUM == 1)
  demo_create();
#elif (TEST_NUM == 2)
  lv_test_theme_1(lv_theme_night_init(210, NULL));
#else
  lv_test_theme_2();
#endif

  while (1)
  {
    lv_task_handler();
    // printf("123\r\n");
    // LCD_ShowString(0, 40, "LCD_W:", RED, WHITE, 16, 0);
    // LCD_ShowIntNum(48, 40, LCD_W, 3, RED, WHITE, 16);
    // LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
    // LCD_ShowIntNum(128, 40, LCD_H, 3, RED, WHITE, 16);
    // LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
    // LCD_ShowString(0, 70, "Increaseing Nun:", RED, WHITE, 16, 0);
    // LCD_ShowFloatNum1(128, 70, t, 4, RED, WHITE, 16);

    // LCD_Fill(0, 0, LCD_W, LCD_H, GRAYBLUE);
    // delay_ms(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, LIGHTGREEN);
    // delay_ms(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, BROWN);
    // delay_ms(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, BLUE);
    // delay_ms(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, MAGENTA);
    // delay_ms(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, RED);
    // delay_ms(1000);
    // t += 0.11;
    // for (j = 0; j < 3; j++)
    // {
    //   for (i = 0; i < 6; i++)
    //   {
    //     LCD_ShowPicture(40 * i, 120 + j * 40, 40, 40, gImage_1);
    //   }
    // }

    // do
    // {
    //   bm_status = iic_read_n_byte(0xa2, 0x02, trdata, 0x07); //测试读取时间、日期
    // } while (bm_status != 0);
    // datajust();
    // Bcd2asc();
    // // printf_sz_hex(asc, 14); //打印时间、日期

    // usart_transmit(trdata, 0x07);

    // delay_ms(1000);
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