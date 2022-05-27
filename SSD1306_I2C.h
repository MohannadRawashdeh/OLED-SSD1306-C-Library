/*
 * SSD1306_I2C.h
 *
 * Created: 5/27/2022 2:07:10 AM
 *  Author: Mohannad Rawashdeh
 */ 

#include "atmel_start.h"

#ifndef SSD1306_I2C_H_
#define SSD1306_I2C_H_

// define the display dimension
#define OLED_SSD1306_128_64		1
#define OLED_SSD1306_128_32		0

#if (OLED_SSD1306_128_64 == 1)
#define WIDTH 128
#define HEIGHT 64
#define PAGES (HEIGHT+7)/8

#define OLED_RST  -1
#define IIC_CMD        0X00
#define IIC_RAM        0X40
#define IIC_ADDRESS    0X3C // could be 3D
struct io_descriptor *I2C_SSD1306_io;
struct _i2c_m_msg i2c_msg_SSD1306;
uint8_t I2C_data_array[2];
#define I2C_data_array_size 2
#define dimension WIDTH * HEIGHT
uint8_t oled_buf[dimension/8];
#endif

#if (OLED_SSD1306_128_32 == 1)
#define WIDTH 128
#define HEIGHT 32
#define PAGES (HEIGHT+7)/8

#define OLED_RST  -1
#define IIC_CMD        0X00
#define IIC_RAM        0X40
#define IIC_ADDRESS    0X3C // could be 3D
struct io_descriptor *I2C_SSD1306_io;
struct _i2c_m_msg i2c_msg_SSD1306;
uint8_t I2C_data_array[2];
#define I2C_data_array_size 2 
#define dimension WIDTH * HEIGHT
uint8_t oled_buf[dimension/8];
#endif


void command(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t Reg);
void data(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t Data);
void er_oled_begin(struct i2c_m_sync_desc *I2C_SSD_OLED);
void er_oled_display(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t* SSD_buffer);
void er_oled_clear(uint8_t* SSD_buffer);
void er_oled_char3216(uint8_t x, uint8_t y, uint8_t chChar, uint8_t* SSD_buffer);
void I2C_Write_Byte(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t value, uint8_t Cmd);
void er_oled_pixel(int x, int y, char color, uint8_t* SSD_buffer);
void er_oled_char(unsigned char x, unsigned char y, char acsii, char size, char mode, uint8_t* buffer);
void I2C_driver_init(struct i2c_m_sync_desc *I2C_SSD_OLED);
void print_num(uint32_t num,uint16_t x_start,uint16_t y_start,uint16_t x_inc);

#endif /* SSD1306_I2C_H_ */