#ifndef flash_proxy_h
#define flash_proxy_h

#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"

void flash_Read(uint32_t addr, uint8_t *buffer, uint32_t buffer_size);
void flash_write(uint32_t addr, uint8_t *buffer, uint32_t buffer_size);

#define MEMORY_INF_ADDR 0x080ff000     /*内存信息地址*/
#define OTA_MEMORY_INF_ADDR 0x080fe000 /*OTA内存信息地址*/

#endif
