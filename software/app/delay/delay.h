#ifndef delay_h
#define delay_h

#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif
