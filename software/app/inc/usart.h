#ifndef usart_h
#define usart_h

#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"

#define USART_RECEIVE_BUFFER_MAX_SIZE 2048


typedef struct
{
  uint8_t   receive_data_buffer[USART_RECEIVE_BUFFER_MAX_SIZE];
  uint32_t  receive_data_size;
}usart_receive_data_t;


void usart1_proxy_init();
void usart_transmit(uint8_t *data_buffer,uint32_t buffer_size);
int fputc(int ch, FILE *f);



extern uint8_t usart_receive_data_buffer[USART_RECEIVE_BUFFER_MAX_SIZE];



#endif



