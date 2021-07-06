#include "at32f4xx_gpio.h"
#include "bm8563.h"
#include "delay.h"
#include "usart.h"

uint8_t twdata[9] = {0x00, 0x00, 0x50, 0x59, 0x23, 0x31, 0x06, 0x12, 0x04}; /*前2个数据用来设置状态寄存器，后 7 个用来设置时间寄存器 */
uint8_t trdata[7] = {0};                                                    /*定义数组用来存储读取的时间数据 */
uint8_t asc[14] = {0};                                                      /*定义数组用来存储转换的 asc 码时间数据，供显示用 */
volatile uint8_t ack = 0;
volatile uint8_t bm_status = 0; //如果BM时钟芯片无应答，则为0,。应答正常则为1.

uint8_t iic_start()
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SDA_SET();
  SCL_SET();
  delay_us(5);

  // if(!GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN))
  // {
  //   return me_IIC_START_ERROR;
  // }

  SDA_RESET();
  delay_us(5);
  SCL_RESET();

  return me_IIC_OK;
}

uint8_t iic_stop()
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SDA_RESET();
  SCL_SET();
  delay_us(5);

  //   if(GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN))
  //   {
  //     return me_IIC_STOP_ERROR;
  //   }

  SDA_SET();
  delay_us(5);

  // if(!GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN))
  // {
  //   return me_IIC_STOP_ERROR;
  // }

  return me_IIC_OK;
}

void iic_ack(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SCL_RESET();
  delay_us(5);
  SDA_RESET();
  SCL_SET();
  delay_us(5);
  SCL_RESET();
  SDA_SET();
  delay_us(5);
}

void iic_nack(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SCL_RESET();
  delay_us(5);
  SDA_SET();
  SCL_SET();
  delay_us(5);
  SCL_RESET();
  delay_us(5);
}

uint8_t iic_wait_ack(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SCL_RESET();
  delay_us(5);
  SDA_SET();
  SCL_SET();
  delay_us(5);

  if (GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN))
  {
    SCL_RESET();
    delay_us(5);
    return me_NACK;
  }
  SCL_RESET();
  delay_us(5);
  return me_ACK;
}

void iic_send_byte(uint8_t luc_Data)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  for (size_t i = 0; i < 8; i++)
  {
    if (luc_Data & 0x80)
    {
      SDA_SET();
    }
    else
    {
      SDA_RESET();
    }

    luc_Data <<= 1;

    SCL_SET();
    delay_us(5);
    SCL_RESET();
    delay_us(5);
  }
}

uint8_t iic_read_byte(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pins = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  uint8_t luc_Data = 0;
  for (size_t i = 0; i < 8; i++)
  {
    luc_Data <<= 1;
    SCL_RESET();
    delay_us(5);
    SCL_SET();
    delay_us(5);

    if (GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN))
    {
      luc_Data |= 0x01;
    }
  }

  return luc_Data;
}

uint8_t iic_write_reg(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress)
{
  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }
  iic_send_byte(luc_SlaveAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  iic_send_byte(luc_RegAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  if (iic_stop() != me_IIC_OK)
  {
    return me_IIC_STOP_ERROR;
  }

  return me_IIC_OK;
}

uint8_t iic_write_reg_data(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t lucRegData)
{
  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }

  iic_send_byte(luc_SlaveAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  iic_send_byte(luc_RegAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  iic_send_byte(lucRegData);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  if (iic_stop() != me_IIC_OK)
  {
    return me_IIC_STOP_ERROR;
  }

  return me_IIC_OK;
}

uint8_t iic_write_n_byte(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t *lpuc_Buf, uint8_t luc_Len)
{
  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }

  iic_send_byte(luc_SlaveAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  iic_send_byte(luc_RegAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  for (size_t i = 0; i < luc_Len; i++)
  {
    iic_send_byte(lpuc_Buf[i]);
    if (i < (luc_Len - 1))
    {
      if (iic_wait_ack() != me_ACK)
      {
        iic_stop();
        return me_IIC_ACK_ERROR;
      }
    }
  }

  if (iic_stop() != me_IIC_OK)
  {
    return me_IIC_STOP_ERROR;
  }

  return me_IIC_OK;
}

uint8_t iic_read_n_byte(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t *lpuc_Buf, uint8_t luc_Len)
{
  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }

  iic_send_byte(luc_SlaveAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  iic_send_byte(luc_RegAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }

  iic_send_byte(luc_SlaveAddress | 1);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  for (size_t i = 0; i < luc_Len; i++)
  {
    lpuc_Buf[i] = iic_read_byte();
    if (i < (luc_Len - 1))
    {
      iic_ack();
    }
  }

  iic_nack();
  if (iic_stop() != me_IIC_OK)
  {
    return me_IIC_STOP_ERROR;
  }

  return me_IIC_OK;
}

uint8_t iic_check_slave_address(uint8_t luc_SlaveAddress)
{
  if (iic_start() != me_IIC_OK)
  {
    return me_IIC_START_ERROR;
  }

  iic_send_byte(luc_SlaveAddress);

  if (iic_wait_ack() != me_ACK)
  {
    iic_stop();
    return me_IIC_ACK_ERROR;
  }

  if (iic_stop() != me_IIC_OK)
  {
    return me_IIC_STOP_ERROR;
  }

  return me_IIC_OK;
}

// /******************************************************************************
// * Function Name --> IIC启动
// * Description   --> SCL高电平期间，SDA由高电平突变到低电平时启动总线
// *                   SCL: __________
// *                                  \__________
// *                   SDA: _____
// *                             \_______________
// * Input         --> none
// * Output        --> none
// * Reaturn       --> none
// ******************************************************************************/
// void iic_start(void)
// {
//   IIC_SDA_SET(); //为SDA下降启动做准备
//   delay_us(1);
//   IIC_SCL_SET(); //在SCL高电平时，SDA为下降沿时候总线启动
//   delay_us(10);
//   IIC_SDA_RESET(); //突变，总线启动
//   delay_us(10);
//   IIC_SCL_RESET();
//   delay_us(2);
// }
// /******************************************************************************
// * Function Name --> IIC停止
// * Description   --> SCL高电平期间，SDA由低电平突变到高电平时停止总线
// *                   SCL: ____________________
// *                                  __________
// *                   SDA: _________/
// * Input         --> none
// * Output        --> none
// * Reaturn       --> none
// ******************************************************************************/
// void iic_stop(void)
// {
//   IIC_SCL_RESET();
//   delay_us(1);
//   IIC_SCL_SET(); //在SCL高电平时，SDA为上升沿时候总线停止
//   delay_us(10);
//   IIC_SDA_SET(); //突变，总线停止
//   delay_us(10);
// }
// /******************************************************************************
// * Function Name --> 主机向从机发送应答信号
// * Description   --> 每从 BM8563 读取一个字节数据后都要发送应答信号
// * Input         --> a：应答信号
// *                      0：应答信号
// *                      1：非应答信号
// * Output        --> none
// * Reaturn       --> none
// ******************************************************************************/
// void IIC_Ack(uint8_t a)
// {
//   if (a)
//     IIC_SDA_SET(); //放上应答信号电平
//   else
//     IIC_SDA_RESET();
//   delay_us(10);
//   IIC_SCL_SET(); //为SCL下降做准备
//   delay_us(10);
//   IIC_SCL_RESET(); //突变，将应答信号发送过去
//   delay_us(2);
// }
// /******************************************************************************
// * Function Name --> 向IIC总线发送一个字节数据
// * Description   --> 向 BM8563 写一个字节的数据
// * Input         --> dat：要发送的数据
// * Output        --> none
// * Reaturn       --> ack：返回应答信号
// ******************************************************************************/
// void iic_send_byte(uint8_t dat)
// {
//   uint8_t i;
//   for (i = 0; i < 8; i++)
//   {
//     if ((dat << i) & 0x80)
//     {
//       IIC_SDA_SET(); //判断发送位，先发送高位
//     }
//     else
//     {
//       IIC_SDA_RESET();
//     }
//     delay_us(1);
//     IIC_SCL_SET(); //为SCL下降做准备
//     delay_us(10);
//     IIC_SCL_RESET(); //突变，将数据位发送过去
//   }                  //字节发送完成，开始接收应答信号
//   delay_us(2);
//   IIC_SDA_SET(); //释放数据线
//   delay_us(2);
//   IIC_SCL_SET(); //为SCL下降做准备
//   delay_us(4);
//   if (IN_SDA()) //读取应答信号
//   {
//     ack = 0;
//   }
//   else
//   {
//     ack = 1;
//   }
//   IIC_SCL_RESET();
//   delay_us(2);
// }

// /******************************************************************************
// * Function Name --> 从IIC总线上读取一个字节数据
// * Description   --> none
// * Input         --> none
// * Output        --> none
// * Reaturn       --> x：读取到的数据
// ******************************************************************************/
// uint8_t iic_read_byte(void)
// {
//   uint8_t i;
//   uint8_t rect = 0;

//   IIC_SDA_SET(); //首先置数据线为高电平

//   for (i = 0; i < 8; i++)
//   {
//     delay_us(1);
//     IIC_SCL_RESET();
//     delay_us(10);
//     IIC_SCL_SET();
//     delay_us(2);
//     rect = rect << 1;
//     if (IN_SDA())
//     {
//       rect = rect + 1;
//     }
//     delay_us(2);
//   }
//   IIC_SCL_RESET();
//   delay_us(2);
//   return rect; //返回读取到的数据
// }

/********************************************************************
函 数 名： GetBM8563(void)
功 能：从 BM8563 的内部寄存器（时间、状态、报警等寄存器）读取数据
说 明：该程序函数用来读取 BM8563 的内部寄存器，譬如时间，报警，状态等寄存器
采用页写的方式，设置数据的个数为 no，no 参数设置为 1 就是单字节方式
调 用：Start_I2C()，SendByte()，RcvByte()，Ack_I2C()，Stop_I2C()
入口参数：sla（BM8563 从地址）， suba（BM8563 内部寄存器地址）
*s（设置读取数据存储的指针）， no（传输数据的个数）
返 回 值：有，用来鉴定传输成功否
***********************************************************************/
uint8_t GetBM8563(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no)
{
  uint8_t i;
  iic_start();
  iic_send_byte(sla);
  if (ack == 0)
    return (0);
  iic_send_byte(suba);
  if (ack == 0)
    return (0);
  iic_start();
  iic_send_byte(sla + 1);
  if (ack == 0)
    return (0);
  for (i = 0; i < no - 1; i++)
  {
    *s = iic_read_byte();
    iic_ack();
    s++;
  }
  *s = iic_read_byte();
  iic_nack();
  iic_stop(); //除最后一个字节外，其他都要从 MASTER 发应答。
  return (1);
}

/********************************************************************
函 数 名：SetBM8563(void)
功 能：设置 BM8563 的内部寄存器（时间，报警等寄存器）
说 明：该程序函数用来设置 BM8563 的内部寄存器，譬如时间，报警，状态等寄存器
采用页写的方式，设置数据的个数为 no，no 参数设置为 1 就是单字节方式
调 用：Start_I2C()，SendByte()，Stop_I2C()
入口参数：sla（BM8563 从地址）， suba（BM8563 内部寄存器地址）
*s（设置初始化数据的指针）， no（传输数据的个数）
返 回 值：有，用来鉴定传输成功否
***********************************************************************/
uint8_t SetBM8563(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no)
{
  uint8_t i;
  iic_start();
  iic_send_byte(sla);
  if (ack == 0)
    return (0);
  iic_send_byte(suba);
  if (ack == 0)
    return (0);
  for (i = 0; i < no; i++)
  {
    iic_send_byte(*s);
    if (ack == 0)
      return (0);
    s++;
  }
  iic_stop();
  return (1);
}

/********************************************************************
函 数 名：void Bcd2asc(void)
功 能：bcd 码转换成 asc 码，供液晶显示用
说 明：
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void Bcd2asc(void)
{
  uint8_t i, j;
  for (j = 0, i = 0; i < 7; i++)
  {
    asc[j++] = (trdata[i] & 0xf0) >> 4 | 0x30; /*格式为: 秒 分 时 日 月 星期 年 */
    asc[j++] = (trdata[i] & 0x0f) | 0x30;
  }
}

/********************************************************************
函 数 名：datajust(void)
功 能：将读出的时间数据的无关位屏蔽掉
说 明：BM8563 时钟寄存器中有些是无关位，可以将无效位屏蔽掉
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void datajust(void)
{
  trdata[0] = trdata[0] & 0x7f;
  trdata[1] = trdata[1] & 0x7f;
  trdata[2] = trdata[2] & 0x3f;
  trdata[3] = trdata[3] & 0x3f;
  trdata[4] = trdata[4] & 0x07;
  trdata[5] = trdata[5] & 0x1f;
  trdata[6] = trdata[6] & 0xff;
}
/********************************************************************
函 数 名：Set_Start_BM8563(void)
功 能：配置启动BM8563
说 明：
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void Set_Start_BM8563(void)
{
  printf_sz_hex(twdata, 9);
  do
  {
    bm_status = iic_write_n_byte(0xa2, 0x00, twdata, 0x09); //设置时间和日期
    printf("123\r\n");
    printf_sz_hex(twdata, 9);
  } while (bm_status != 0);
  printf("321\r\n");
}
