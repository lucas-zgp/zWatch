#include "my_assert.h"



void my_assert(uint8_t condition,char* file_name,uint32_t line_no)
{
  
  if(!condition)
  {
    printf( "\n[Debug]Assert failed: %s, line %u\n",file_name,line_no);
    abort();
  }
}


