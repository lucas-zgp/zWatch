#ifndef __ASSERT_H
#define __ASSERT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


void my_assert(uint8_t condition,char* file_name,uint32_t line_no);

#endif 