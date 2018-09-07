#include "png_dec.h"
#include "bmp_enc.h"
#include "user_ram.h"

#include "upng.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PNG_PATH "0:/p%d.png"

#define UPNG_FLIP_Y //竖直翻转

/*
 * flip the image
 * Set to flip the image data along the horizontal axis and vertical axis
 * in_buffer:  ptr to the in data
 * width:      iamge width
 * height:     image height
 * out_buffer: output data buffer
 */
void raw_data_convert(uint8_t *in_buffer, uint32_t width, uint32_t height, uint8_t *out_buffer)
{	
	uint32_t pixel_num = width * height;	
	uint32_t color_idx = 0;	

	uint32_t depth = 3;

	for (uint32_t y = 0; y != height; ++y) 
	{
		for (uint32_t x = 0; x != width; ++x) 
		{
			out_buffer[color_idx++] = in_buffer[(height - y - 1) * width * depth + x * depth + (depth - 0 - 1)];
			out_buffer[color_idx++] = in_buffer[(height - y - 1) * width * depth + x * depth + (depth - 1 - 1)];
			out_buffer[color_idx++] = in_buffer[(height - y - 1) * width * depth + x * depth + (depth - 2 - 1)];			
		}
	}

	if(color_idx != pixel_num*3)
	{
		printf("convert err!\n");
	}
}

/*
 * convert png to bmp
 * note: read png data from file by fatfs and decode to a malloc buffer
 * png_path: png file path
 */
int dec_png(const char * png_path)
{
	upng_t* upng;
	unsigned width, height, depth;
	unsigned x, y, d;

	upng = upng_new_from_file(png_path);
	if(upng != NULL)
	{
		upng_decode(upng);
		if (upng_get_error(upng) != UPNG_EOK) 
		{
			printf("error: %d %d\n", upng_get_error(upng), upng_get_error_line(upng));
			upng_free(upng);
			return 0;
		}
	}
	else
	{
		printf("open file err!\n");
		return 0;
	}

	width = upng_get_width(upng);
	height = upng_get_height(upng);
	depth = upng_get_bpp(upng) / 8;

	printf("w:%d h:%d bpp:%d size:%d\n", width, height, upng_get_bpp(upng), upng_get_size(upng));
//	printf("format:	%d\n", upng_get_format(upng));

	if (upng_get_format(upng) == UPNG_RGB8) 
	{
		uint8_t *header = bmp_buffer;
		encodeToBMP(header, width, height);

		raw_data_convert((uint8_t *)upng_get_buffer(upng), width, height, &bmp_buffer[54]);

		//write to a file
		uint32_t length = width * height * depth;
		static uint32_t item_id = 0;
		writeToBMP(bmp_buffer, length + 54, item_id++);
	}
	else
	{
		printf("[PNG] unsupport color format.\n");
	}

	upng_free(upng);
	return 0;
}

/*
 * convert png to bmp
 * note: read png data from buffer and decode to a preallocated buffer
 * png_buffer:  ptr to png source data
 * png_size:    png source length
 * bmp_buffer:  ptr to bmp out data buffer
 * bmp_buf_len: out buffer length
 */
int dec_png_buf(uint8_t *png_buffer, uint32_t png_size, uint8_t *bmp_buffer, uint32_t bmp_buf_len)
{
	upng_t* upng;
	unsigned width, height, depth;

	upng = upng_new_from_bytes(png_buffer, png_size);
	if(upng != NULL)
	{
		upng_decode_to_buffer(upng, bmp_buffer, bmp_buf_len);
		if (upng_get_error(upng) != UPNG_EOK) 
		{
			printf("error: %d %d\n", upng_get_error(upng), upng_get_error_line(upng));
			upng_free(upng);
			return 0;
		}
	}
	else
	{
		printf("open file err!\n");
		return 0;
	}

	width = upng_get_width(upng);
	height = upng_get_height(upng);
	depth = upng_get_bpp(upng) / 8;

	printf("w:%d h:%d bpp:%d size:%d\n", width, height, upng_get_bpp(upng), upng_get_size(upng));
//	printf("format:	%d\n", upng_get_format(upng));

	if (upng_get_format(upng) == UPNG_RGB8) 
	{
		uint8_t *header = bmp_buffer;
		encodeToBMP(header, width, height);

		raw_data_convert((uint8_t *)upng_get_buffer(upng), width, height, &header[54]);

		//write to a bmp file
//		uint32_t length = width * height * depth;
//		static uint32_t item_id = 0;
//		writeToBMP(bmp_buffer, length + 54, item_id++);
	}
	else
	{
		printf("[PNG] unsupport color format.\n");
	}

	upng_free(upng);
	return 0;
}


/*
 * test main()
 */
void png_dec_test()
{
	char png_path[20];

	uint8_t i = 2;
	for(i=1; i<=10; i++)
	{
		printf("i = %d\n", i);
		sprintf(png_path, PNG_PATH, i);	
		dec_png(png_path, sdram_buffer_2, sizeof(sdram_buffer_2));
		printf("\n");
	}
}
