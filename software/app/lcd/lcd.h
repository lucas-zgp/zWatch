#ifndef __LCD_H
#define __LCD_H

void LCD_Fill(unsigned short int xsta, unsigned short int ysta, unsigned short int xend, unsigned short int yend, unsigned short int color);  //指定区域填充颜色
void LCD_DrawPoint(unsigned short int x, unsigned short int y, unsigned short int color);                                                     //在指定位置画一个点
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color);      //在指定位置画一条线
void LCD_DrawRectangle(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color); //在指定位置画一个矩形
void Draw_Circle(unsigned short int x0, unsigned short int y0, unsigned char r, unsigned short int color);                                    //在指定位置画一个圆

void LCD_ShowChinese(unsigned short int x, unsigned short int y, unsigned char *s, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode);      //显示汉字串
void LCD_ShowChinese12x12(unsigned short int x, unsigned short int y, unsigned char *s, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode); //显示单个12x12汉字
void LCD_ShowChinese16x16(unsigned short int x, unsigned short int y, unsigned char *s, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode); //显示单个16x16汉字
void LCD_ShowChinese24x24(unsigned short int x, unsigned short int y, unsigned char *s, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode); //显示单个24x24汉字
void LCD_ShowChinese32x32(unsigned short int x, unsigned short int y, unsigned char *s, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode); //显示单个32x32汉字

void LCD_ShowChar(unsigned short int x, unsigned short int y, unsigned char num, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode);        //显示一个字符
void LCD_ShowString(unsigned short int x, unsigned short int y, const unsigned char *p, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode); //显示字符串
unsigned int mypow(unsigned char m, unsigned char n);                                                                                                                           //求幂
void LCD_ShowIntNum(unsigned short int x, unsigned short int y, unsigned short int num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey);  //显示整数变量
void LCD_ShowFloatNum1(unsigned short int x, unsigned short int y, float num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey);            //显示两位小数变量

void LCD_ShowPicture(unsigned short int x, unsigned short int y, unsigned short int length, unsigned short int width, const unsigned char pic[]); //显示图片
void LCD_ShowPicture_1(unsigned short int x, unsigned short int y,
					   unsigned short int length, unsigned short int width,
					   unsigned short int *pic);
//画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40      //棕色
#define BRRED 0XFC07      //棕红色
#define GRAY 0X8430       //灰色
#define DARKBLUE 0X01CF   //深蓝色
#define LIGHTBLUE 0X7D7C  //浅蓝色
#define GRAYBLUE 0X5458   //灰蓝色
#define LIGHTGREEN 0X841F //浅绿色
#define LGRAY 0XC618      //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 0XA651  //浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12     //浅棕蓝色(选择条目的反色)

#endif
