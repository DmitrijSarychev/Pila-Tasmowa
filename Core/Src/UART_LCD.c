/*
 * UART_LCD.c
 *
 *  Created on: Nov 9, 2023
 *      Author: Dima
 */
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

void LCD_Set_Cursor(uint8_t x, uint8_t y)
{
	x--;
	y--;
	if (x > 15 || x <0)
		{
			x = 15;
		}
	if (y > 1 || y < 0)
		{
			y = 1;
		}

	uint8_t CursorPosition = 128 + y*64 + x; //Change the position (128) of the cursor to (y) row (1,2), position x (1-16)
	uint8_t Command = COMMAND;
	HAL_UART_Transmit(&huart2, &Command, 1, UART_TIMEOUT);
	HAL_UART_Transmit(&huart2, &CursorPosition, 1, UART_TIMEOUT);
}

void LCD_Write(const uint8_t *pData, uint8_t Size)
{
	HAL_UART_Transmit(&huart2, pData, Size, UART_TIMEOUT);
}

void LCD_Setting(uint8_t Data)
{
	uint8_t SettingMode = SETTING_MODE;
	uint8_t *pData = &Data;
	HAL_UART_Transmit(&huart2, &SettingMode, 1, UART_TIMEOUT);
	HAL_UART_Transmit(&huart2, pData, 1, UART_TIMEOUT);
}

void LCD_SetRGB(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t SettingMode = SETTING_MODE;
	if (r > 29) r = 29;
	if (g > 29) g = 29;
	if (b > 29) b = 29;
	r = RGB_RED + r;
	g = RGB_GREEN + g;
	b = RGB_BLUE + b;
	uint8_t *pR = &r;
	uint8_t *pG = &g;
	uint8_t *pB = &b;
	HAL_UART_Transmit(&huart2, &SettingMode, 1, UART_TIMEOUT);
	HAL_UART_Transmit(&huart2, pR, 1, UART_TIMEOUT);

	HAL_UART_Transmit(&huart2, &SettingMode, 1, UART_TIMEOUT);
	HAL_UART_Transmit(&huart2, pG, 1, UART_TIMEOUT);

	HAL_UART_Transmit(&huart2, &SettingMode, 1, UART_TIMEOUT);
	HAL_UART_Transmit(&huart2, pB, 1, UART_TIMEOUT);
}

void LCD_SetContrast (uint8_t Contrast)
{
	  LCD_Setting(CONTRAST);
	  LCD_Write(&Contrast, 1);
}

void LCD_Init(void)
{
	//HAL_GPIO_WritePin(LCD_Reset_GPIO_Port, LCD_Reset_Pin, 0);
	//HAL_Delay(500);
	//HAL_GPIO_WritePin(LCD_Reset_GPIO_Port, LCD_Reset_Pin, 1);
	HAL_Delay(1000);
	LCD_Setting(BAUD8);			//Set BAUD rate to 115200 b/s
	HAL_Delay(100);
	HAL_UART_DeInit(&huart2);
	huart2.Init.BaudRate = 115200;
	HAL_UART_Init(&huart2);
	LCD_Setting(CLR_DISP);		//Clear Display
	LCD_Setting(SYS_MSG_OFF);		//Disable system messages
	LCD_SetContrast(0);			//Set contrast to max
	LCD_SetRGB(29, 29, 29);		//Set backlight RGB to white (values 0-29)
}
