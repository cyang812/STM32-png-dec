#include "bmp_enc.h"
#include "jpg_dec.h"
#include "png_dec.h"
#include "user_ram.h"
#include "user_display.h"
#include "ff.h"
#include <string.h>

FIL pic_file;

void encodeToBMP(uint8_t *bmp_header, uint32_t xsize, uint32_t ysize)
{
	uint8_t header[54] =
	{
    	0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    
    uint32_t file_size = (uint32_t)xsize * (uint32_t)ysize * 3 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    uint32_t width = xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    uint32_t height = ysize;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

	memmove(bmp_header, header, sizeof(header));
}

/*
 * param idx: item id
 */
uint8_t writeToBMP(const uint8_t *data, uint32_t data_len, uint32_t item_id)
{
    uint8_t filename_buf[100];
    sprintf((char *)filename_buf, "0:/out%04d.bmp", item_id);

    uint8_t res = f_open(&pic_file, (char const *)filename_buf, FA_WRITE|FA_CREATE_ALWAYS);
    if(res)
    {
        printf("unable creat file!\n");
        return 0;
    }
    else
    {
    	//bmp
        f_write(&pic_file, data, data_len, NULL);
    }

    printf("write file succ, file name = %s, len = %d\n", filename_buf, pic_file.fsize);
    f_close(&pic_file);
}

void DisplayBMP(uint8_t pic_type)
{
	if(pic_type == 1) //png
	{
		dec_png_buf(source, length, sdram_buffer_2, sizeof(sdram_buffer_2));
	}
	else //jpg
	{
		dec_jpg_buf(source, length, sdram_buffer_2);
	}

	LCD_DisplayBmp(50, 50, sdram_buffer_2);
	printf("pic type = %s\n", pic_type==1 ? "PNG":"JPG");
}