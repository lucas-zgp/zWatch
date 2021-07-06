#include "flash.h"
#include "my_assert.h"


#define PAGE_SIZE          0x800
#define FLASH_BASE_ADDR    0x08000000
#define FLASH_SIZE         1024*1024


static void flash_write_nocheck(uint32_t addr, uint8_t *buffer, uint32_t buffer_size); 
static uint8_t flash_read_byte(uint32_t addr);


static uint8_t flash_read_byte(uint32_t addr)
{
  return *(uint8_t*)addr; 
}


static void flash_write_nocheck(uint32_t addr, uint8_t *buffer, uint32_t buffer_size)   
{ 			 		 
  for (size_t i = 0; i < buffer_size; i++)
  {
    FLASH_ProgramByte(addr,buffer[i]);
    addr++;
  }
} 


void flash_Read(uint32_t addr, uint8_t *buffer, uint32_t buffer_size)
{
  for (size_t i = 0; i < buffer_size; i++)
  {
    buffer[i]=flash_read_byte(addr);
    addr++;
  }
}


uint8_t  page_buffer[PAGE_SIZE] = {0}; /*用于保存当前扇区已经存在的内容*/
void flash_write(uint32_t addr,uint8_t* buffer,uint32_t buffer_size)
{
  uint32_t offset_addr            = 0;   /*存储偏移地址*/
  uint32_t page_pos               = 0;   /*第几个扇区*/
  uint32_t page_offset            = 0;   /*扇区内偏移地址*/
  uint32_t page_remain_size       = 0;   /*扇区剩余空间*/
  uint32_t i = 0;

  if((addr < FLASH_BASE_ADDR) || (addr >=(FLASH_BASE_ADDR + FLASH_SIZE)))
  {
    my_assert(0,__FILE__,__LINE__);
    return;
  }
    
  /* 解锁FLASH */
  FLASH_Unlock();
  offset_addr      = addr - FLASH_BASE_ADDR;  
  page_pos         = offset_addr/PAGE_SIZE;     /*第几个扇区*/
  page_offset      = offset_addr%PAGE_SIZE;     /*扇区内偏移地址*/
  page_remain_size = PAGE_SIZE - page_offset;		/*扇区剩余空间大小*/   
    
  if(buffer_size <= page_remain_size)
  {
    page_remain_size = buffer_size; /*不大于该扇区范围*/
  }

  while(1) 
  {	
    flash_Read(page_pos*PAGE_SIZE+FLASH_BASE_ADDR,page_buffer,PAGE_SIZE);//读出整个扇区的内容

    for (i = 0; i < page_remain_size; i++)
    {
      if(page_buffer[page_offset+i] != 0xFF)
      {
        break;//需要擦除
      }
    }
    
    if(i < page_remain_size)//需要擦除
    {
      FLASH_ErasePage(page_pos*PAGE_SIZE+FLASH_BASE_ADDR);//擦除这个扇区

      for(i=0;i<page_remain_size;i++)//复制
      {
        page_buffer[i+page_offset] = buffer[i]; //保留页偏移地址以前的数据	  
      }

      flash_write_nocheck(page_pos*PAGE_SIZE+FLASH_BASE_ADDR,page_buffer,PAGE_SIZE);//写入整个扇区  
    }
    else //不需要擦除
    {
      flash_write_nocheck(addr,buffer,page_remain_size);//写已经擦除了的,直接写入扇区剩余区间. 	
    }
        
    if(buffer_size == page_remain_size)
    {
      break;//写入结束了
    }
    else//写入未结束
    {
      page_pos++;				                  //扇区地址增1
      page_offset    = 0;			         	  //偏移位置为0 	 
      buffer         += page_remain_size; //指针偏移
      addr           += page_remain_size;	//写地址偏移	   
      buffer_size    -= page_remain_size;	//字节(16位)数递减
      if(buffer_size > PAGE_SIZE)
      {
        page_remain_size = PAGE_SIZE;//下一个扇区还是写不完
      }
      else 
      {
        page_remain_size=buffer_size;//下一个扇区可以写完了
      }
    }	 
  }

  /*上锁FLASH*/
  FLASH_Lock();
}







