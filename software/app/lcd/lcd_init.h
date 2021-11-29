#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#define USE_HORIZONTAL 0 //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif

//-----------------LCD端口定义----------------

/*PA5*/
#define LCD_SCLK_Clr() GPIO_WriteBit(GPIOA, GPIO_Pins_5, Bit_RESET); //SCL=SCLK
#define LCD_SCLK_Set() GPIO_WriteBit(GPIOA, GPIO_Pins_5, Bit_SET)

/*PA7*/
#define LCD_MOSI_Clr() GPIO_WriteBit(GPIOA, GPIO_Pins_7, Bit_RESET); //SDA=MOSI
#define LCD_MOSI_Set() GPIO_WriteBit(GPIOA, GPIO_Pins_7, Bit_SET)

/*PA6*/
#define LCD_RES_Clr() GPIO_WriteBit(GPIOA, GPIO_Pins_6, Bit_RESET); //RES
#define LCD_RES_Set() GPIO_WriteBit(GPIOA, GPIO_Pins_6, Bit_SET)

/*PA4*/
#define LCD_DC_Clr() GPIO_WriteBit(GPIOA, GPIO_Pins_4, Bit_RESET); //DC
#define LCD_DC_Set() GPIO_WriteBit(GPIOA, GPIO_Pins_4, Bit_SET)

/*PB0*/
#define LCD_CS_Clr() GPIO_WriteBit(GPIOB, GPIO_Pins_0, Bit_RESET); //CS
#define LCD_CS_Set() GPIO_WriteBit(GPIOB, GPIO_Pins_0, Bit_SET)

/*PA8*/
#define LCD_BLK_Clr() GPIO_WriteBit(GPIOA, GPIO_Pins_8, Bit_RESET); //BLK
#define LCD_BLK_Set() GPIO_WriteBit(GPIOA, GPIO_Pins_8, Bit_SET)

void LCD_GPIO_Init(void);                                                                                         //初始化GPIO
void LCD_Writ_Bus(unsigned char dat);                                                                             //模拟SPI时序
void LCD_WR_DATA8(unsigned char dat);                                                                             //写入一个字节
void LCD_WR_DATA(unsigned short int dat);                                                                         //写入两个字节
void LCD_WR_REG(unsigned char dat);                                                                               //写入一个指令
void LCD_Address_Set(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2); //设置坐标函数
void LCD_Init(void);                                                                                              //LCD初始化
#endif
