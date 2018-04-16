#ifndef PNG_DEC_H
#define PNG_DEC_H

#include <stdint.h>

extern int dec_png(const char * png_path);
extern int dec_png_buf(uint8_t *png_buffer, uint32_t png_size, uint8_t *bmp_buffer, uint32_t bmp_buf_len);
extern void png_dec_test(void);

#endif
