#include "spi.h"
#include "at32f4xx_dma.h"
#include "at32f4xx_spi.h"

/**
 * @brief  spi1初始化
 * @note   SPI工作在mode0 即 CPOL = 0, CPHA = 0
 *         SPI_CPOLY等于0,即SPI空闲时的时钟电平为低电平
 *         SPI_CPHA等于0,即SPI在时钟的第1个边沿开始采样
 * @retval None
 */
void spi_init()
{
    GPIO_InitType GPIO_InitStructure;
    SPI_InitType SPI_InitStructure;

    /*初始化时钟*/
    RCC_APB2PeriphClockCmd(SPI1_RCC | SPI1_SCK_RCC | SPI1_MOSI_RCC, ENABLE);

    /*初始化引脚*/
    GPIO_InitStructure.GPIO_Pins = SPI1_SCK_PIN;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pins = SPI1_MISO_PIN;
    // GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    // GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

    /*SPI1参数配置*/
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_TX_HALFDUPLEX;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;
    SPI_InitStructure.SPI_CPOLY = 0;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_Init(SPI1, &SPI_InitStructure);

    /*使能SPI1*/
    SPI_Enable(SPI1, ENABLE);

    spi_read_write_byte(0xff);
}

/**
 * @brief  SPI读写一个半字数据
 * @note   SPI工作在全双工模式下,写入一个数据的同时也是读取到一个数据
 * @param  write_byte:需要写入的数据
 * @retval 返回当前读取到的数据
 */
unsigned char spi_read_write_byte(unsigned char write_byte)
{
    unsigned char read_byte = 0;

    /*读取SPI_STS寄存器的TE(bit1)*/
    /*发送缓冲区为非空  TE = 0*/
    /*发送缓冲区为空    TE = 1*/
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET) /*判断发送区是否非空*/
    {
    }

    SPI_I2S_TxData(SPI1, write_byte); /*发送 1 Byte数据*/

    // /*读取SPI_STS寄存器的RNE(bit0)*/
    // /*接收缓冲区为非空  RNE = 0*/
    // /*接收缓冲区为空    RNE = 1*/
    // while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RNE) == RESET) /*判断接收区是否非空*/
    // {
    // }

    // read_byte = SPI_I2S_RxData(SPI1); /*返回最近接收到的1 Byte数据*/

    /*读取SPI_STS寄存器的BSY(bit7)*/
    /*SPI不忙  BSY = 0*/
    /*SPI忙    BSY = 1*/
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY) == SET) /*判断SPI1是否忙*/
    {
    }

    return read_byte;
}

volatile unsigned char read_ing_flag = 0;

unsigned char SPI_MASTER_Buffer_Rx[BufferSize];

unsigned char SPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

void spi_dma_init()
{
    GPIO_InitType GPIO_InitStructure;
    SPI_InitType SPI_InitStructure;
    DMA_InitType DMA_InitStructure;

    /*初始化时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(SPI1_RCC | SPI1_SCK_RCC | SPI1_MOSI_RCC, ENABLE);

    /*初始化引脚*/
    GPIO_InitStructure.GPIO_Pins = SPI1_SCK_PIN;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);

    //    GPIO_InitStructure.GPIO_Pins = SPI1_MISO_PIN;
    //    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    //    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //    GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

    /*SPI1参数配置*/
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
    SPI_InitStructure.SPI_CPOLY = 0;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_8;
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Enable(SPI1, ENABLE); /*使能SPI1*/

    /*spi dma rx 配置*/
    DMA_Reset(DMA1_Channel2);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)0x4001300C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)SPI_MASTER_Buffer_Rx;
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    /*NVIC配置*/
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_INTConfig(DMA1_Channel2, DMA_INT_TC, ENABLE); /*使能发送完成中断*/

    SPI1->CTRL2 |= 1 << 0;                    /*启动接收缓冲区DMA 当该位被设置时,RNE标志一旦被置位就发出DMA请求*/
    DMA_ChannelEnable(DMA1_Channel2, ENABLE); /*使能接受区DMA*/

    /*spi dma tx 配置*/
    DMA_Reset(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)0x4001300C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)SPI_MASTER_Buffer_Tx;
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    /*NVIC配置*/
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_INTConfig(DMA1_Channel3, DMA_INT_TC, ENABLE);  /*开启 DMA1_Channel3 传输完成中断*/
    DMA_INTConfig(DMA1_Channel3, DMA_INT_ERR, ENABLE); /*开启 DMA1_Channel3 传输错误中断*/

    SPI1->CTRL2 |= 1 << 1;                     /*启动发送缓冲区DMA 当该位被设置时,TE标志一旦被置位就发出DMA请求*/
    DMA_ChannelEnable(DMA1_Channel3, DISABLE); /*使能发送去DMA*/
}

/*******************************************************************************
 * Function Name  : SPI1_Send
 * Description    : SPI1的DMA方式发送
 * Input          : SPI1_TX_Buff[SPI1_SendBufferSize]
 * Output         : None
 * Return         : None
 * Attention      : 关闭DMA通道3之前必须等待TXE为1，等待忙标志为0
 *******************************************************************************/
void spi1_dma_send(unsigned char *buff, unsigned short int len)
{
    DMA1_Channel3->CHCTRL &= ~(1 << 0); /*关闭DMA通道2*/

    DMA1_Channel3->CPBA = (unsigned int)0x4001300C; /*外设地址*/
    DMA1_Channel3->CMBA = (unsigned int)buff;       /*mem地址*/
    DMA1_Channel3->TCNT = len;                      /*传输长度为0-65535*/
    DMA1_Channel3->CHCTRL = 0;                      /*CHCTRL寄存器清0*/
    DMA1_Channel3->CHCTRL = (0x00 << 14) |          /*非存储器到存储器模式*/
                            (0x03 << 12) |          /*通道优先级高*/
                            (0x00 << 10) |          /*存储器数据宽度8bit*/
                            (0x00 << 8) |           /*外设数据宽度8bit*/
                            (0x01 << 7) |           /*执行存储器地址增量操作*/
                            (0x00 << 6) |           /*不执行外设地址增量操作*/
                            (0x00 << 5) |           /*不执行循环操作*/
                            (0x01 << 4) |           /*从存储器读*/
                            (0x01 << 3) |           /*允许传输错误中断*/
                            (0x01 << 1) |           /*允许传输完成中断*/
                            (0x01 << 0);            /*通道使能*/
}

/*******************************************************************************
 * Function Name  : SPI1_Recive
 * Description    : SPI1的DMA方式接收
 * Input          : None
 * Output         : SPI1_RX_Buff[SPI1_ReciveBufferSize]
 * Return         : None
 * Attention             : 必须要先关闭通道2,然后再配置通道2的参数
 *******************************************************************************/
void spi1_dma_recive(unsigned char *buff, unsigned short int len)
{
    DMA1_Channel2->CHCTRL &= ~(1 << 0); /*关闭DMA通道2*/

    DMA1_Channel2->CPBA = (unsigned int)0x4001300C; /*外设地址*/
    DMA1_Channel2->CMBA = (unsigned int)buff;       /*mem地址*/
    DMA1_Channel2->TCNT = len;                      /*传输长度*/
    DMA1_Channel2->CHCTRL = (0x00 << 14) |          /*非存储器到存储器模式*/
                            (0x03 << 12) |          /*通道优先级 最高*/
                            (0x00 << 10) |          /*存储器数据宽度 8位*/
                            (0x00 << 8) |           /*外设数据宽度 8位*/
                            (0x01 << 7) |           /*执行存储器地址增量操作*/
                            (0x00 << 6) |           /*不执行外设地址增量操作*/
                            (0x00 << 5) |           /*不执行循环操作*/
                            (0x00 << 4) |           /*从外设读*/
                            (0x00 << 3) |           /*禁止 TE 中断*/
                            (0x00 << 2) |           /*禁止 HT 中断*/
                            (0x01 << 1) |           /*允许 TC 中断*/
                            (0x01 << 0);            /*通道使能*/
}

void DMA1_Channel2_IRQHandler()
{
    if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC2);
    }
}

void DMA1_Channel3_IRQHandler()
{
    if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC3);

        /*读取SPI_STS寄存器的BSY(bit7)*/
        /*SPI不忙  BSY = 0*/
        /*SPI忙    BSY = 1*/
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY) == SET) /*判断SPI1是否忙*/
        {
        }
        //        BGT_SPI_CS_DISABLE;
        read_ing_flag = 0;
    }

    if (DMA_GetFlagStatus(DMA1_FLAG_ERR3) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_FLAG_ERR3);
    }
}