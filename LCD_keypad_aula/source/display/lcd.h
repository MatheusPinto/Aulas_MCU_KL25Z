/*
 * lcd.h
 *
 *  Created on: 15 de jul de 2019
 *      Author: Matheus_Pinto
 */
#include "fsl_gpio.h"


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_4BITMODE 0x00
#ifndef LCD_4BITMODE
#define LCD_8BITMODE 0x10
#endif
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct{
	GPIO_Type *base;
	uint32_t pin;
}lcdPin_t;

typedef struct{
#ifdef LCD_8BITMODE
	lcdPin_t data[8];
#else
	lcdPin_t data[4];
#endif
	lcdPin_t rs;
	lcdPin_t en;
}lcdBus_t;

typedef struct{
	uint8_t cols;
	uint8_t lines;
	uint8_t charsize;
	lcdBus_t bus;
}lcdConfig_t;

void lcd_Init(lcdConfig_t *config);

/********** high level commands, for the user! */
void lcd_Clear();

void lcd_Home();

void lcd_SetCursor(uint8_t col, uint8_t row);

// Turn the display on/off (quickly)
void lcd_NoDisplay();

void lcd_Display();

// Turns the underline cursor on/off
void lcd_NoCursor();

void lcd_Cursor();

// Turn on and off the blinking cursor
void lcd_NoBlink();

void lcd_Blink();

// These commands scroll the display without changing the RAM
void lcd_ScrollDisplayLeft(void);

void lcd_ScrollDisplayRight(void);

// This is for text that flows Left to Right
void lcd_LeftToRight(void);

// This is for text that flows Right to Left
void lcd_RightToLeft(void);

// This will 'right justify' text from the cursor
void lcd_Autoscroll(void);

// This will 'left justify' text from the cursor
void lcd_NoAutoscroll(void);

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_CreateChar(uint8_t location, const uint8_t charmap[]);

void lcd_CreateBigNumsChars(void);

void lcd_WriteBigNum(uint8_t col, uint8_t num);

void lcd_WriteString(char *str);

/*********** mid level commands, for sending data/cmds */

void lcd_Command(uint8_t value);
size_t lcd_Write(uint8_t value);
