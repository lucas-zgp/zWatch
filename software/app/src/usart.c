#include "usart.h"

#include <string.h>
#include <stdlib.h>

uint8_t usart_receive_data_buffer[USART_RECEIVE_BUFFER_MAX_SIZE] = {0};

void usart1_proxy_init()
{
  DMA_InitType   DMA_InitStructure;
  GPIO_InitType GPIO_InitStructure;
  USART_InitType USART_InitStructure;

  /*GPIO 配置*/
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);	
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins     = GPIO_Pins_9; 
  GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  /*串口1 配置*/
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART1, &USART_InitStructure); 


  /*串口1 DMA1_Channel5配置*/
  RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);
  DMA_Reset(DMA1_Channel5);
  DMA_DefaultInitParaConfig(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&USART1->DT;  
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)usart_receive_data_buffer; 
  DMA_InitStructure.DMA_Direction           = DMA_DIR_PERIPHERALSRC;  
  DMA_InitStructure.DMA_BufferSize          = USART_RECEIVE_BUFFER_MAX_SIZE;
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc           = DMA_MEMORYINC_ENABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_MemoryDataWidth     = DMA_MEMORYDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_Mode                = DMA_MODE_CIRCULAR; 
  DMA_InitStructure.DMA_Priority            = DMA_PRIORITY_VERYHIGH;
  DMA_InitStructure.DMA_MTOM                = DMA_MEMTOMEM_DISABLE;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);


  /*NVIC配置*/
  NVIC_InitType NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*使能*/
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  USART_INTConfig(USART1, USART_INT_IDLEF, ENABLE);
  USART_Cmd(USART1, ENABLE);   
  DMA_ChannelEnable(DMA1_Channel5, ENABLE);
}


/*串口发送函数*/
void usart_transmit(uint8_t *data_buffer,uint32_t buffer_size)
{
  taskENTER_CRITICAL();
  for (size_t i = 0; i < buffer_size; i++)
  {
    USART_SendData(USART1,data_buffer[i]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TDE) == RESET);
  }
  taskEXIT_CRITICAL();
}

/*printf 重映射*/
int fputc(int ch, FILE *f)
{
  USART_SendData(USART1, ch);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TRAC) == RESET);    
  return ch;
}






