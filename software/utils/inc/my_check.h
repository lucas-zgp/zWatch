#ifndef __my_check_h
#define __my_check_h

#include "stdint.h"


uint16_t crc16(uint8_t *input_data_buffer, uint16_t data_lenth);
uint8_t checksum(uint8_t *input_data_buffer, uint16_t data_lenth);





#endif 