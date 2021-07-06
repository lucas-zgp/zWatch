#include "delay.h"

void delay_ms(uint16_t nms)
{
  while (nms)
  {
    delay_us(1000);
    nms--;
  }
}

void delay_us(uint32_t us)
{
  volatile unsigned char i = 0;
  while (us--)
  {
    i = 25;
    while (i--)
      ;
  };
}
