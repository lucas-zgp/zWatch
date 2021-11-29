#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"
#include "delay.h"

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(unsigned short int xsta, unsigned short int ysta,
			  unsigned short int xend, unsigned short int yend,
			  unsigned short int color)
{
	unsigned short int i, j;
	LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //设置显示范围
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(unsigned short int x, unsigned short int y, unsigned short int color)
{
	LCD_Address_Set(x, y, x, y); //设置光标位置
	LCD_WR_DATA(color);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	unsigned short int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; //画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}

/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(unsigned short int x0, unsigned short int y0, unsigned char r, unsigned short int color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		LCD_DrawPoint(x0 - b, y0 - a, color); //3
		LCD_DrawPoint(x0 + b, y0 - a, color); //0
		LCD_DrawPoint(x0 - a, y0 + b, color); //1
		LCD_DrawPoint(x0 - a, y0 - b, color); //2
		LCD_DrawPoint(x0 + b, y0 + a, color); //4
		LCD_DrawPoint(x0 + a, y0 - b, color); //5
		LCD_DrawPoint(x0 + a, y0 + b, color); //6
		LCD_DrawPoint(x0 - b, y0 + a, color); //7
		a++;
		if ((a * a + b * b) > (r * r)) //判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(unsigned short int x, unsigned short int y, unsigned char num, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	unsigned char temp, sizex, t, m = 0;
	unsigned short int i, TypefaceNum; //一个字符所占字节大小
	unsigned short int x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';									 //得到偏移后的值
	LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); //设置光标位置
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i]; //调用6x12字体
		else if (sizey == 16)
			temp = ascii_1608[num][i]; //调用8x16字体
		else if (sizey == 24)
			temp = ascii_2412[num][i]; //调用12x24字体
		else if (sizey == 32)
			temp = ascii_3216[num][i]; //调用16x32字体
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode) //非叠加模式
			{
				if (temp & (0x01 << t))
					LCD_WR_DATA(fc);
				else
					LCD_WR_DATA(bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else //叠加模式
			{
				if (temp & (0x01 << t))
					LCD_DrawPoint(x, y, fc); //画一个点
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(unsigned short int x, unsigned short int y, const unsigned char *p, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	while (*p != '\0')
	{
		LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
unsigned int mypow(unsigned char m, unsigned char n)
{
	unsigned int result = 1;
	while (n--)
		result *= m;
	return result;
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(unsigned short int x, unsigned short int y, unsigned short int num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp;
	unsigned char enshow = 0;
	unsigned char sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(unsigned short int x, unsigned short int y, float num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp, sizex;
	unsigned short int num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(unsigned short int x, unsigned short int y,
					 unsigned short int length, unsigned short int width,
					 const unsigned char pic[])
{
	unsigned short int i, j;
	unsigned int k = 0;
	LCD_Address_Set(x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			LCD_WR_DATA8(pic[k * 2]);
			LCD_WR_DATA8(pic[k * 2 + 1]);
			k++;
		}
	}
}

// //把指定区域的显示缓冲区写入屏幕
// void LCD_Color_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *buf)
// {
// 	uint16_t num;
// 	num = (x1 - x0 + 1) * (y1 - y0 + 1);
// 	LCD_Address_Set(x0, y0, x1, y1);
// 	LCD_CS_OUT(0);

// 	LCD_SPI->CR1 &= ~SPI_CR1_SPE; //失能SPI
// 	LCD_SPI->CR1 |= SPI_CR1_DFF;  //设置SPI16位传输模式
// 	LCD_SPI->CR1 |= SPI_CR1_SPE;  //使能SPI

// 	LCD_DMA_Transfer16Bit((uint8_t *)buf, num, DMA_MEMINC_ENABLE); //启用DMA发送

// 	//其余部分见HAL_SPI_TxCpltCallback()函数
// }

void LCD_ShowPicture_1(unsigned short int x, unsigned short int y,
					   unsigned short int length, unsigned short int width,
					   unsigned short int *pic)
{
	unsigned short int i, j;
	unsigned int k = 0;
	LCD_Address_Set(x, y, length, width);
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < length; j++)
		{
			LCD_WR_DATA(pic[k]);
			// LCD_WR_DATA8(pic[k * 2]);
			// LCD_WR_DATA8(pic[k * 2 + 1]);
			k++;
		}
	}
}

// void LCD_Fill(unsigned short int xsta, unsigned short int ysta,
// 			  unsigned short int xend, unsigned short int yend,
// 			  unsigned short int color)
// {
// 	unsigned short int i, j;
// 	LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //设置显示范围
// 	for (i = ysta; i < yend; i++)
// 	{
// 		for (j = xsta; j < xend; j++)
// 		{
// 			LCD_WR_DATA(color);
// 		}
// 	}
// }