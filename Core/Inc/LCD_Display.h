/*
 * LCD_Display.h
 *
 *  Created on: Jun 26, 2023
 *      Author: Dima
 */

#ifndef INC_LCD_DISPLAY_H_
#define INC_LCD_DISPLAY_H_

#include "main.h"
#include "i2c.h"
#include "Pila.h"

#define LCD_ADDRESS     0x7C
#define RGB_ADDRESS     0xC0


/*!
 *  color define
 */
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

/*!
 *   commands
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/*!
 *   flags for display entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/*!
 *   flags for display on/off control
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/*!
 *   flags for display/cursor shift
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/*!
 *   flags for function set
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x8DOTS 0x00


	void LCD_init();
	void LCD_home();
	void LCD_display();
	void LCD_command(uint8_t);
	void LCD_send(uint8_t *data, uint8_t len);
	void LCD_setReg(uint8_t addr, uint8_t data);
	void LCD_setRGB(uint8_t r, uint8_t g, uint8_t b);
	void LCD_setCursor(uint8_t col, uint8_t row);
	void LCD_clear();
	void LCD_BlinkLED();
	void LCD_noBlinkLED();
	void LCD_write_char(uint8_t value);
	void LCD_send_string(const char *str);
	void LCD_stopBlink();
	void LCD_blink();
	void LCD_noCursor();
	void LCD_cursor();
	void LCD_scrollDisplayLeft();
	void LCD_scrollDisplayRight();
	void LCD_leftToRight();
	void LCD_rightToLeft();
	void LCD_noAutoscroll();
	void LCD_autoscroll();
	void LCD_customSymbol(uint8_t location, uint8_t charmap[]);
	void LCD_setColorWhite();
	void LCD_begin(uint8_t cols, uint8_t lines);


#endif /* INC_LCD_DISPLAY_H_ */
