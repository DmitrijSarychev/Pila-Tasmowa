/*
 * UART_LCD.h
 *
 *  Created on: Nov 9, 2023
 *      Author: Dima
 *      Command cheat sheet:
 ASCII / DEC / HEX
 '|'    / 124 / 0x7C - Put into setting mode
 Ctrl+c / 3 / 0x03 - Change width to 20
 Ctrl+d / 4 / 0x04 - Change width to 16
 Ctrl+e / 5 / 0x05 - Change lines to 4
 Ctrl+f / 6 / 0x06 - Change lines to 2
 Ctrl+g / 7 / 0x07 - Change lines to 1
 Ctrl+h / 8 / 0x08 - Software reset of the system
 Ctrl+i / 9 / 0x09 - Enable/disable splash screen
 Ctrl+j / 10 / 0x0A - Save currently displayed text as splash
 Ctrl+k / 11 / 0x0B - Change baud to 2400bps
 Ctrl+l / 12 / 0x0C - Change baud to 4800bps
 Ctrl+m / 13 / 0x0D - Change baud to 9600bps
 Ctrl+n / 14 / 0x0E - Change baud to 14400bps
 Ctrl+o / 15 / 0x0F - Change baud to 19200bps
 Ctrl+p / 16 / 0x10 - Change baud to 38400bps
 Ctrl+q / 17 / 0x11 - Change baud to 57600bps
 Ctrl+r / 18 / 0x12 - Change baud to 115200bps
 Ctrl+s / 19 / 0x13 - Change baud to 230400bps
 Ctrl+t / 20 / 0x14 - Change baud to 460800bps
 Ctrl+u / 21 / 0x15 - Change baud to 921600bps
 Ctrl+v / 22 / 0x16 - Change baud to 1000000bps
 Ctrl+w / 23 / 0x17 - Change baud to 1200bps
 Ctrl+x / 24 / 0x18 - Change the contrast. Follow Ctrl+x with number 0 to 255. 120 is default.
 Ctrl+y / 25 / 0x19 - Change the TWI address. Follow Ctrl+x with number 0 to 255. 114 (0x72) is default.
 Ctrl+z / 26 / 0x1A - Enable/disable ignore RX pin on startup (ignore emergency reset)
 '-'    / 45 / 0x2D - Clear display. Move cursor to home position.
        / 128-157 / 0x80-0x9D - Set the primary backlight brightness. 128 = Off, 157 = 100%.
        / 158-187 / 0x9E-0xBB - Set the green backlight brightness. 158 = Off, 187 = 100%.
        / 188-217 / 0xBC-0xD9 - Set the blue backlight brightness. 188 = Off, 217 = 100%.
         For example, to change the baud rate to 115200 send 124 followed by 18.
 '+'    / 43 / 0x2B - Set Backlight to RGB value, follow + by 3 numbers 0 to 255, for the r, g and b values.
         For example, to change the backlight to yellow send + followed by 255, 255 and 0.

 *********************************************
 *****************Cursor position cheat sheet:
 *********************************************

 For the most part, it is pretty straight forward on how to control the cursor.
 Be warned though, the "line" numbers are a little strange (0, 64, 20, 84).
 There are four "parts" to the two commands needed.

 First, the command character (254), simply write this with:

 OpenLCD.write(254);

 Second, a single write that consists of adding up three important numbers.

 OpenLCD.write(changePosition + line + position);

 changePosition never changes (haha), it is the command for the serLCD to know we want to change position. It is always 128.

 Then the "line" number is either 0, 64, 20 or 84.

 These correspond to each line like so:

 Line 1 = 0
 Line 2 = 64
 Line 3 = 20
 Line 4 = 84

 Then the "position" is a number from 0-15 or 0-19 (depending on which screen you are using).

 For the 16x2 models, you will only need to set line 1 or line 2, so here's an example:

 Jump to line 1, position 9
 OpenLCD.write(254); //Send command character
 OpenLCD.write(128 + 64 + 9); //Change the position (128) of the cursor to 2nd row (64), position 9 (9)

 On a 20x4 model, you can try all four lines. For example:

 Jump to line 3, position 4
 OpenLCD.write(254); //Send command character
 OpenLCD.write(128 + 20 + 4); //Change the position (128) of the cursor to 3rd row (20), position 4 (4)

 And another 20x4 example:

 Jump to line 4, position 18
 OpenLCD.write(254); //Send command character
 OpenLCD.write(128 + 84 + 18); //Change the position (128) of the cursor to 4th line (84), position 18 (18)
 */

#ifndef INC_UART_LCD_H_
#define INC_UART_LCD_H_

#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

#include "Pila.h"
#include "UART_LCD.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SETTING_MODE 	0x7C	//'|'    / 124 / 0x7C - Put into setting mode
#define WIDTH20			0x03	//Ctrl+c / 3 / 0x03 - Change width to 20
#define WIDTH16			0x04	//Ctrl+d / 4 / 0x04 - Change width to 16
#define LINES4			0x05	//Ctrl+e / 5 / 0x05 - Change lines to 4
#define LINES2			0x06	//Ctrl+f / 6 / 0x06 - Change lines to 2
#define LINES1			0x07	//Ctrl+g / 7 / 0x07 - Change lines to 1
#define LCD_RESET		0x08	//Ctrl+h / 8 / 0x08 - Software reset of the system
#define SPLASH_SCR		0x09	//Ctrl+i / 9 / 0x09 - Enable/disable splash screen
#define SPLASH_SAVE		0x0A	//Ctrl+j / 10 / 0x0A - Save currently displayed text as splash
#define BAUD1			0x0B	//Ctrl+k / 11 / 0x0B - Change baud to 2400bps
#define BAUD2			0x0C	//Ctrl+l / 12 / 0x0C - Change baud to 4800bps
#define BAUD3			0x0D	//Ctrl+m / 13 / 0x0D - Change baud to 9600bps
#define BAUD4			0x0E	//Ctrl+n / 14 / 0x0E - Change baud to 14400bps
#define BAUD5			0x0F	//Ctrl+o / 15 / 0x0F - Change baud to 19200bps
#define BAUD6			0x10	//Ctrl+p / 16 / 0x10 - Change baud to 38400bps
#define BAUD7			0x11	//Ctrl+q / 17 / 0x11 - Change baud to 57600bps
#define BAUD8			0x12	//Ctrl+r / 18 / 0x12 - Change baud to 115200bps
#define BAUD9			0x13	//Ctrl+s / 19 / 0x13 - Change baud to 230400bps
#define BAUD10			0x14	//Ctrl+t / 20 / 0x14 - Change baud to 460800bps
#define BAUD11			0x15	//Ctrl+u / 21 / 0x15 - Change baud to 921600bps
#define BAUD12			0x16	//Ctrl+v / 22 / 0x16 - Change baud to 1000000bps
#define BAUD13			0x17	//Ctrl+w / 23 / 0x17 - Change baud to 1200bps
#define CONTRAST		0x18	//Ctrl+x / 24 / 0x18 - Change the contrast. Follow Ctrl+x with number 0 to 255. 0 is default.
#define I2C_ADDR		0x19	//Ctrl+y / 25 / 0x19 - Change the TWI address. Follow Ctrl+x with number 0 to 255. 114 (0x72) is default.
#define RX_IGNORE		0x1A	//Ctrl+z / 26 / 0x1A - Enable/disable ignore RX pin on startup (ignore emergency reset)
#define CLR_DISP		0x2D	//'-'    / 45 / 0x2D - Clear display. Move cursor to home position.
#define SYS_MSG_ON		0x2E	//		 / 46 / 0x2E - Enable system messages (contrast/backlight/baud change)
#define SYS_MSG_OFF		0x2F	//		 / 47 / 0x2F - Disable system messages (contrast/backlight/baud change)

#define COMMAND			0xFE	//		 / 254 / 0xFE - Send command Byte (used to change cursor position)

#define UART_TIMEOUT	100		//

#define RGB_RED			128		//		 / 128 / 0x80 - Start address for RED backlight colour change
#define RGB_GREEN		158		//		 / 158 / 0x9E - Start address for RED backlight colour change
#define RGB_BLUE		188		//		 / 188 / 0xBC - Start address for RED backlight colour change

#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3




void LCD_Set_Cursor(uint8_t x, uint8_t y);
void LCD_Write(const uint8_t *pData, uint8_t Size);
void LCD_Setting(uint8_t Data);
void LCD_SetRGB(uint8_t r, uint8_t g, uint8_t b);
void LCD_SetContrast (uint8_t Contrast);
void LCD_Init(void);


#endif /* INC_UART_LCD_H_ */
