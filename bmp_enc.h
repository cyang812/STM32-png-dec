#ifndef BMP_ENC_H
#define BMP_ENC_H

#include <stdint.h>

extern void    encodeToBMP(uint8_t *bmp_header, uint32_t xsize, uint32_t ysize);
extern uint8_t writeToBMP(const uint8_t *data, uint32_t data_len, uint32_t item_id);
extern void    DisplayBMP(uint8_t pic_type);

#endif
