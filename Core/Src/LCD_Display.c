/*
 * LCD_Display.c
 *
 *  Created on: Jun 26, 2023
 *      Author: Dima
 */
#include "cmsis_os.h"
#include "LCD_Display.h"

	uint8_t LCD_showfunction;
	uint8_t LCD_showcontrol;
	uint8_t LCD_showmode;
	uint8_t LCD_initialized;
	uint8_t LCD_numlines,_currline;
	uint8_t LCD_lcdAddr;
	uint8_t LCD_RGBAddr;
	uint8_t LCD_cols;
	uint8_t LCD_rows;
	uint8_t LCD_backlightval;

void LCD_init()
{
	LCD_showfunction = LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS;
	LCD_begin(LCD_cols, LCD_rows);
}

void LCD_begin(uint8_t cols, uint8_t lines)
{
    ///< SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    ///< according to datasheet, we need at least 40ms after power rises above 2.7V
    ///< before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    HAL_Delay(50);

    ///< this is according to the hitachi HD44780 datasheet
    ///< page 45 figure 23

    ///< Send function set command sequence
    LCD_command(LCD_FUNCTIONSET | LCD_showfunction);
    HAL_Delay(5);  // wait more than 4.1ms

	///< second try
    LCD_command(LCD_FUNCTIONSET | LCD_showfunction);
    HAL_Delay(5);

    ///< third go
    LCD_command(LCD_FUNCTIONSET | LCD_showfunction);


    ///< turn the display on with no cursor or blinking default
    LCD_showcontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_display();

    ///< clear it off
    LCD_clear();

    ///< Initialize to default text direction (for romance languages)
    LCD_showmode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    ///< set the entry mode
    LCD_command(LCD_ENTRYMODESET | LCD_showmode);


    ///< backlight init
    LCD_setReg(REG_MODE1, 0);
    ///< set LEDs controllable by both PWM and GRPPWM registers
    LCD_setReg(REG_OUTPUT, 0xFF);
    ///< set MODE2 values
    ///< 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
    LCD_setReg(REG_MODE2, 0x20);

    LCD_setColorWhite();
}

void LCD_command(uint8_t value)
{
    uint8_t data[2] = {0x80, value};
    LCD_send(data, 2);
}

void LCD_send(uint8_t *data, uint8_t len)
{
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data, len, 10);
	//HAL_I2C_Master_Transmit_DMA(&hi2c3, LCD_ADDRESS, data, len);
}

void LCD_display()
{
	LCD_showcontrol |= LCD_DISPLAYON;
	LCD_command(LCD_DISPLAYCONTROL | LCD_showcontrol);
}

void LCD_clear()
{
    LCD_command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
    HAL_Delay(2);          // this command takes a long time!
}

void LCD_setReg(uint8_t addr, uint8_t data)
{
	uint8_t send_data[2] = {addr, data};
	HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDRESS, send_data, sizeof(send_data), 20);
	//HAL_I2C_Master_Transmit_DMA(&hi2c3, RGB_ADDRESS, send_data, sizeof(send_data));
}

void LCD_setRGB(uint8_t r, uint8_t g, uint8_t b)
{
	LCD_setReg(REG_RED, r);
	LCD_setReg(REG_GREEN, g);
	LCD_setReg(REG_BLUE, b);
}

void LCD_setCursor(uint8_t col, uint8_t row)
{
	if (row == 0)	{col = col|0x80;}
	else			{col = col|0xC0;}

    uint8_t data[2] = {0x80, col};

    LCD_send(data, 2);
}

void LCD_write_char(uint8_t value)
{
    uint8_t data[2] = {0x40, value};
    LCD_send(data, 2);
}

void LCD_send_string(const char *str)
{
	uint8_t i;
	uint8_t LCD_text[32];
	for(i = 0; str[i] != '\0';i++)
	{
		//LCD_write_char(str[i]);
		if (str[i + 1] == '\0') {LCD_text[i * 2] = 0x40;}
		else {LCD_text[i * 2] = 0xC0;}
		LCD_text[i * 2 + 1] = str[i];
	}

	LCD_send(LCD_text, i * 2);

}

void LCD_BlinkLED(void)
{
    ///< blink period in seconds = (<reg 7> + 1) / 24
    ///< on/off ratio = <reg 6> / 256
    LCD_setReg(0x07, 0x17);  // blink every second
    LCD_setReg(0x06, 0x7f);  // half on, half off
}

void LCD_noBlinkLED(void)
{
	LCD_setReg(0x07, 0x00);
	LCD_setReg(0x06, 0xff);
}

void LCD_stopBlink()
{
	LCD_showcontrol &= ~LCD_BLINKON;
	LCD_command(LCD_DISPLAYCONTROL | LCD_showcontrol);
}

void LCD_blink()
{
	LCD_showcontrol |= LCD_BLINKON;
    LCD_command(LCD_DISPLAYCONTROL | LCD_showcontrol);
}

void LCD_noCursor()
{
	LCD_showcontrol &= ~LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | LCD_showcontrol);
}

void LCD_cursor() {
	LCD_showcontrol |= LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | LCD_showcontrol);
}

void LCD_scrollDisplayLeft(void)
{
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_scrollDisplayRight(void)
{
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_leftToRight(void)
{
	LCD_showmode |= LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | LCD_showmode);
}

void LCD_rightToLeft(void)
{
	LCD_showmode &= ~LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | LCD_showmode);
}

void LCD_noAutoscroll(void)
{
	LCD_showmode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | LCD_showmode);
}

void LCD_autoscroll(void)
{
	LCD_showmode |= LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | LCD_showmode);
}

void LCD_customSymbol(uint8_t location, uint8_t charmap[])
{

    location &= 0x7; // we only have 8 locations 0-7
    LCD_command(LCD_SETCGRAMADDR | (location << 3));


    uint8_t data[9];
    data[0] = 0x40;
    for(int i=0; i<8; i++)
    {
        data[i+1] = charmap[i];
    }
    LCD_send(data, 9);
}

void LCD_home()
{
    LCD_command(LCD_RETURNHOME);        // set cursor position to zero
    HAL_Delay(2);        // this command takes a long time!
}

void LCD_setColorWhite()
{
	LCD_setRGB(255, 255, 255);
}
