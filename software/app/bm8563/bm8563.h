#ifndef bm8563_h
#define bm8563_h

#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"

#define SDA_PORT GPIOB      /*BN8563 SDA PORT*/
#define SDA_PIN GPIO_Pins_6 /*BN8563 SDA PIN*/

#define SCL_PORT GPIOB      /*BN8563 SCL PORT*/
#define SCL_PIN GPIO_Pins_5 /*BN8563 SCL PIN*/

#define SDA_SET() GPIO_WriteBit(SDA_PORT, SDA_PIN, Bit_SET)
#define SDA_RESET() GPIO_WriteBit(SDA_PORT, SDA_PIN, Bit_RESET)

#define SCL_SET() GPIO_WriteBit(SCL_PORT, SCL_PIN, Bit_SET)
#define SCL_RESET() GPIO_WriteBit(SCL_PORT, SCL_PIN, Bit_RESET)

uint8_t iic_wait_ack(void);
uint8_t iic_start(void);
uint8_t iic_stop(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t luc_Data);
uint8_t iic_read_byte(void);

uint8_t iic_write_reg(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress);
uint8_t iic_write_reg_data(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t lucRegData);
uint8_t iic_write_n_byte(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t *lpuc_Buf, uint8_t luc_Len);
uint8_t iic_read_n_byte(uint8_t luc_SlaveAddress, uint8_t luc_RegAddress, uint8_t *lpuc_Buf, uint8_t luc_Len);
uint8_t iic_check_slave_address(uint8_t luc_SlaveAddress);

enum IICErrorCode
{
    me_IIC_OK = 0,
    me_IIC_START_ERROR,
    me_IIC_STOP_ERROR,
    me_IIC_ACK_ERROR,
};

enum IICAck
{
    me_NACK = 0,
    me_ACK,
};

void Set_Start_BM8563(void);

extern uint8_t twdata[9]; /*前2个数据用来设置状态寄存器，后 7 个用来设置时间寄存器 */
extern uint8_t trdata[7]; /*定义数组用来存储读取的时间数据 */
extern uint8_t asc[14];   /*定义数组用来存储转换的 asc 码时间数据，供显示用 */


extern volatile uint8_t ack ;
extern volatile uint8_t bm_status ; //如果BM时钟芯片无应答，则为0,。应答正常则为1.


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
uint8_t GetBM8563(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no);
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
uint8_t SetBM8563(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no);
/********************************************************************
函 数 名：void Bcd2asc(void)
功 能：bcd 码转换成 asc 码，供液晶显示用
说 明：
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void Bcd2asc(void);
/********************************************************************
函 数 名：datajust(void)
功 能：将读出的时间数据的无关位屏蔽掉
说 明：BM8563 时钟寄存器中有些是无关位，可以将无效位屏蔽掉
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void datajust(void);
/********************************************************************
函 数 名：Set_Start_BM8563(void)
功 能：配置启动BM8563
说 明：
调 用：
入口参数：
返 回 值：无
***********************************************************************/
void Set_Start_BM8563(void);
#endif
