#ifndef __bsp_spi_h
#define __bsp_spi_h

#include "at32f4xx_crc.h"
#include "at32f4xx_gpio.h"
#include "at32f4xx_spi.h"

#define SPI1_RCC RCC_APB2PERIPH_SPI1 /*SPI1时钟*/

#define SPI1_SCK_RCC RCC_APB2PERIPH_GPIOA /*SP1 SCK 时钟*/
#define SPI1_SCK_PORT GPIOA               /*SP1 SCK 端口*/
#define SPI1_SCK_PIN GPIO_Pins_5          /*SP1 SCK 引脚*/

// #define SPI1_MISO_RCC RCC_APB2PERIPH_GPIOA /*SP1 MISO 时钟*/
// #define SPI1_MISO_PORT GPIOA               /*SP1 MISO 端口*/
// #define SPI1_MISO_PIN GPIO_Pins_6          /*SP1 MISO 引脚*/

#define SPI1_MOSI_RCC RCC_APB2PERIPH_GPIOA /*SP1 MOSI 时钟*/
#define SPI1_MOSI_PORT GPIOA               /*SP1 MOSI 端口*/
#define SPI1_MOSI_PIN GPIO_Pins_7          /*SP1 MOSI 引脚*/

/**
 * @brief  spi1初始化
 * @note
 * @retval None
 */
void spi_init(void);

/**
 * @brief  SPI读写一个半字数据
 * @note   SPI工作在全双工模式下,写入一个数据的同时也是读取到一个数据
 * @param  write_byte:需要写入的数据
 * @retval 返回当前读取到的数据
 */
// unsigned short int spi_read_write_byte(unsigned short int write_byte);
unsigned char spi_read_write_byte(unsigned char write_byte);

void spi_dma_init(void);
void spi1_dma_send(unsigned char *buff, unsigned short int len);
void spi1_dma_recive(unsigned char *buff, unsigned short int len);

#define BufferSize 16
extern unsigned char SPI_MASTER_Buffer_Rx[BufferSize];

extern unsigned char SPI_MASTER_Buffer_Tx[BufferSize];

extern volatile unsigned char read_ing_flag;
#endif