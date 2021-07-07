#include "timer.h"
#include "lvgl.h"
#include "usart.h"

void timer6_init()
{
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6, ENABLE);
  TMR_TimerBaseInitType TMR_TimeBaseStructure = {0};
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
  TMR_TimeBaseStructure.TMR_DIV = 100 - 1;
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
  TMR_TimeBaseStructure.TMR_Period = 2400 - 1;
  TMR_TimeBaseStructure.TMR_ClockDivision = TMR_CKD_DIV1;

  TMR_TimeBaseInit(TMR6, &TMR_TimeBaseStructure);
  NVIC_InitType NVIC_InitStructure = {0};
  NVIC_InitStructure.NVIC_IRQChannel = TMR6_GLOBAL_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TMR_INTConfig(TMR6, TMR_INT_Overflow, ENABLE);
  TMR_Cmd(TMR6, ENABLE);
}

void TMR6_GLOBAL_IRQHandler(void)
{
  static unsigned short int num = 0;
  if (TMR_GetINTStatus(TMR6, TMR_INT_Overflow) == SET)
  {
    // if (++num >= 1000)
    // {
    //   printf("1\r\n");
    //   num = 0;
    // }

    lv_tick_inc(1); //lvgl 的 1ms 心跳
  }
  TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
}