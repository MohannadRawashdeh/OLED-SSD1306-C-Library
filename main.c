/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 
 * OLED display SSD1306 I2C driver demo code, for 128*64 and 128*32 display
 */

#include "atmel_start.h"
#include <hal_gpio.h>
#include <hal_delay.h>
#include "SSD1306_I2C.h"
int main(void)
{
	atmel_start_init();
	 er_oled_begin(&I2C0);
	 er_oled_clear(oled_buf);
	 er_oled_display(&I2C0,oled_buf);
	 delay_ms(3000);
	 command(&I2C0,0xa7);//--set Negative display
	 delay_ms(3000);
	 command(&I2C0,0xa6);//--set normal display
	 delay_ms(10);
	 // print the word 'scan' on the display
	 er_oled_char(16,0,'s',16,1,oled_buf);
	 er_oled_char(24,0,'c',16,1,oled_buf);
	 er_oled_char(32,0,'a',16,1,oled_buf);
	 er_oled_char(40,0,'n',16,1,oled_buf);
	 
	 er_oled_char(80,0,'1',16,1,oled_buf);
	 er_oled_char(88,0,'8',16,1,oled_buf);
	 er_oled_char(96,0,':',16,1,oled_buf);
	 er_oled_char(104,0,'4',16,1,oled_buf);
	 er_oled_char(112,0,'7',16,1,oled_buf);
	 er_oled_display(&I2C0,oled_buf);
	 // to control the blink for the time dot
	 bool dot_blink=1;
	 delay_ms(3000);
	 // print the numbers from 0 to 65000 with 500ms delay
		for (uint16_t num_try=0;num_try<65000;num_try+=10)
		{
			// set the x start, then y start point with the X axis increment value
			print_num(num_try,16,16,16);
			delay_ms(500);
			dot_blink =!dot_blink;
			if(dot_blink){er_oled_char(96,0,':',16,1,oled_buf);}
			else {er_oled_char(96,0,' ',16,1,oled_buf);}
		}
	 er_oled_display(&I2C0,oled_buf);
	 er_oled_clear(oled_buf);
	while (true) {
		delay_ms(500);
		gpio_toggle_pin_level(LED0);
	}
}
