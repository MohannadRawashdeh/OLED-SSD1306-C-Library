/*
 * SSD1306_I2C.c
 *
 * Created: 5/27/2022 2:13:57 AM
 *  Author: Mohannad Rawashdeh
 */ 

#include "atmel_start.h"
#include "SSD1306_I2C.h"
#include "OLED_font.h"

void I2C_driver_init(struct i2c_m_sync_desc *I2C_SSD_OLED)
{
	
	//i2c_m_sync_get_io_descriptor(&I2C0, &I2C_SSD1306_io);
	i2c_m_sync_get_io_descriptor(I2C_SSD_OLED, &I2C_SSD1306_io);
	i2c_m_sync_enable(I2C_SSD_OLED);
	i2c_m_sync_set_slaveaddr(I2C_SSD_OLED, IIC_ADDRESS, I2C_M_SEVEN);
	//io_write(I2C0_io, (uint8_t *)"Hello World!", 12);
}
void I2C_Write_Byte(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t value, uint8_t Cmd)
{
	// prepare the packet first
	I2C_data_array[0]=Cmd;
	I2C_data_array[1]=value;
	//use this
	//io_write(I2C_SSD1306_io,I2C_data_array, I2C_data_array_size);
	//or use this
	i2c_msg_SSD1306.addr=IIC_ADDRESS;
	i2c_msg_SSD1306.flags=I2C_M_STOP;
	i2c_msg_SSD1306.buffer=I2C_data_array;
	i2c_msg_SSD1306.len=I2C_data_array_size;
	
	i2c_m_sync_transfer(I2C_SSD_OLED, &i2c_msg_SSD1306);
}

void command(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t Reg)
{
	I2C_Write_Byte(I2C_SSD_OLED,Reg, IIC_CMD);
}
void data(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t Data)
{
	I2C_Write_Byte(I2C_SSD_OLED,Data, IIC_RAM);
}

void er_oled_begin(struct i2c_m_sync_desc *I2C_SSD_OLED)
{
	/*
	pinMode(OLED_RST, OUTPUT);
	digitalWrite(OLED_RST, HIGH);
	delay(10);
	digitalWrite(OLED_RST, LOW);
	delay(10);
	digitalWrite(OLED_RST, HIGH);
	*/
	I2C_driver_init(I2C_SSD_OLED);
	delay_ms(20);
	command(I2C_SSD_OLED,0xae);//--turn off oled panel
	
	command(I2C_SSD_OLED,0xd5);//--set display clock divide ratio/oscillator frequency
	command(I2C_SSD_OLED,0x80);//--set divide ratio

	command(I2C_SSD_OLED,0xa8);//--set multiplex ratio(1 to 64)
	command(I2C_SSD_OLED,0x3f);//--1/64 duty

	command(I2C_SSD_OLED,0xd3);//-set display offset
	command(I2C_SSD_OLED,0x00);//-not offset

	command(I2C_SSD_OLED,0x8d);//--set Charge Pump enable/disable
	command(I2C_SSD_OLED,0x14);//--set(0x10) disable

	command(I2C_SSD_OLED,0x40);//--set start line address
	//all good
	command(I2C_SSD_OLED,0xa6);//--set normal display

	command(I2C_SSD_OLED,0xa4);//Disable Entire Display On

	command(I2C_SSD_OLED,0xa1);//--set segment re-map 128 to 0

	command(I2C_SSD_OLED,0xC8);//--Set COM Output Scan Direction 64 to 0

	command(I2C_SSD_OLED,0xda);//--set com pins hardware configuration
	command(I2C_SSD_OLED,0x12);

	command(I2C_SSD_OLED,0x81);//--set contrast control register
	command(I2C_SSD_OLED,0xcf);

	command(I2C_SSD_OLED,0xd9);//--set pre-charge period
	command(I2C_SSD_OLED,0xf1);

	command(I2C_SSD_OLED,0xdb);//--set vcomh
	command(I2C_SSD_OLED,0x40);
	
	command(I2C_SSD_OLED,0xaf);//--turn on oled panel
}

void er_oled_clear(uint8_t* SSD_buffer)
{
	int i;
	for(i = 0;i < WIDTH * HEIGHT / 8;i++)
	{
		SSD_buffer[i] = 0;
	}
}

void er_oled_display(struct i2c_m_sync_desc *I2C_SSD_OLED,uint8_t* SSD_buffer)
{    uint8_t page,i;
	for (page = 0; page < PAGES; page++) {
		command(I2C_SSD_OLED,0xB0 + page);/* set page address */
		command(I2C_SSD_OLED,0x00);   /* set low column address */
		command(I2C_SSD_OLED,0x10);  /* set high column address */
		for(i = 0; i< WIDTH; i++ ) {
			data(I2C_SSD_OLED,SSD_buffer[i+page*WIDTH]);// write data one
		}
	}
}

void er_oled_pixel(int x, int y, char color, uint8_t* SSD_buffer)
{
	if(x > WIDTH || y > HEIGHT)return ;
	if(color)
	SSD_buffer[x+(y/8)*WIDTH] |= 1<<(y%8);
	else
	SSD_buffer[x+(y/8)*WIDTH] &= ~(1<<(y%8));
}

void er_oled_char3216(uint8_t x, uint8_t y, uint8_t chChar, uint8_t* buffer)
{
	uint8_t i, j;
	uint8_t chTemp = 0, y0 = y, chMode = 0;

	for (i = 0; i < 64; i++) {
		chTemp = Font3216[chChar - 0x30][i];
		for (j = 0; j < 8; j++) {
			chMode = chTemp & 0x80? 1 : 0;
			er_oled_pixel(x, y, chMode, buffer);
			chTemp <<= 1;
			y++;
			if ((y - y0) == 32) {
				y = y0;
				x++;
				break;
			}
		}
	}
}
void er_oled_char(unsigned char x, unsigned char y, char acsii, char size, char mode, uint8_t* buffer)
{
	unsigned char i, j, y0=y;
	char temp;
	unsigned char ch = acsii - ' ';
	for(i = 0;i<size;i++) {

	if(mode)temp = Font1608[ch][i];
	else temp = ~Font1608[ch][i];
		
		for(j =0;j<8;j++)
		{
			if(temp & 0x80) er_oled_pixel(x, y, 1, buffer);
			else er_oled_pixel(x, y, 0, buffer);
			temp <<= 1;
			y++;
			if((y-y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

void print_num(uint32_t num,uint16_t x_start,uint16_t y_start,uint16_t x_inc)
{
	uint32_t num32=num;
	uint8_t num_size=1;
	// first determine the number of digit in this number
	if (num32<10){num_size=1;}
	else if (num32<100){num_size=2;}
	else if (num32<1000){num_size=3;}
	else if (num32<10000){num_size=4;}
	else if (num32<100000){num_size=5;}
	else if (num32<1000000){num_size=6;}
	else {num_size=1;}// more than 1M scan ! impossible
	// array to save the string.
	char sbuf[num_size];
	// now change the number to string using sprintf function
	sprintf(sbuf,"%lu",num32);
	// now print the results on the screen starting from x_start,ystart
	for (uint8_t num_count=0;num_count<num_size;num_count++)
	{
		er_oled_char3216(x_start, y_start, sbuf[num_count], oled_buf);
		x_start+=x_inc;
	}
	// done!
	er_oled_display(&I2C0,oled_buf);
}