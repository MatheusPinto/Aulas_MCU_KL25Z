/**
 * @file	lcd.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2021
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A generic driver for LCD with HD44780 controller.
 * It is necessary to include the "mcu_general_config.h" file with
 * specific implementations of:
 *     - MCU_PortSet(portPinsRegister, portPinMask) and
 *     - MCU_PortClear(portPinsRegister, portPinMask).
 * It is also necessary the definition of types:
 *     - portPinsRegister_t - the register type used by "MCU_PortSet"
 *                            and "MCU_PortClear";
 *     - portPinMask_t - the mask type used by "MCU_PortSet"
 *                       and "MCU_PortClear" to indicate the pin
 *                       position to be referred.
 *
 * Supported OSes:
 *
 *   - FreeRTOS.
 *
 */

#include "generic_drivers/lcd/lcd.h"
#include "libraries/delay/delay.h"
#include "libraries/emb_util/emb_util.h"
#ifdef __FREERTOS_H
#include "FreeRTOS.h"
#include "semphr.h"
#endif /* __FREERTOS_H */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< Information to create new big number characters, stored in non-volatile memory.*/
const uint8_t _bigNumsCodes[] = {0b00000001,//0
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00011111,
								 0b00000000,
								 0b00011111,//1
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00000000,
								 0b00011111,//2
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000000,
								 0b00000001,//3
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000000,
								 0b00011111,//4
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00011111,
								 0b00000000,
								 0b00011111,//5
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00000001,
								 0b00011111,
								 0b00000000,
								 0b00011111,//6
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00000000,
								 0b00011111,//7
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00010000,
								 0b00011111,
								 0b00000000};

/*!< Commands to print big numbers in screen.*/
static const uint8_t _bigNumCommands[10][4] =
{
		{0x01, 0x02, 0x4C, 0x00}, //nr. 0
		{0x20, 0x7C, 0x20, 0x7C}, //nr. 1
		{0x04, 0x05, 0x4C, 0x5F}, //nr. 2
		{0x06, 0x05, 0x5F, 0x00}, //nr. 3
		{0x4C, 0x00, 0x20, 0x03}, //nr. 4
		{0x07, 0x04, 0x5F, 0x00}, //nr. 5
		{0x07, 0x04, 0x4C, 0x00}, //nr. 5
		{0x06, 0x02, 0x20, 0x03}, //nr. 7
		{0x07, 0x05, 0x4C, 0x00}, //nr. 8
		{0x07, 0x05, 0x20, 0x03}  //nr. 9
};



/*!< Enumeration to identify objects creation.*/
enum{
	kLcdObjectIsHandle,
	kLcdObjectIsConfig,
};


/*!
 * @brief LCD handle structure used internally
 *
 */
struct lcdHandle_s{
	/*!< The pointer to the configuration structure passed by the user.*/
	lcdConfig_t* config;
	/*!< The display function command that will be send to the LCD controller.*/
	uint8_t displayfunction;
	/*!< The display control command that will be send to the LCD controller.*/
	uint8_t displaycontrol;
	/*!< The display mode command that will be send to the LCD controller.*/
	uint8_t displaymode;
	/*!< The row offsets list determined by the number of LCD rows and columns.*/
	uint8_t row_offsets[4];
#ifdef __FREERTOS_H
#ifdef LCD_REENTRANT_ACCESS
	/*!< The mutex used for mutual exclusion in API calls.*/
	xSemaphoreHandle lcdAccessMutex;
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */
};


#ifdef LCD_STATIC_OBJECTS_CREATION
/*!< The static list of configuration structures that is returned to the LCD API.*/
static lcdConfig_t g_lcdConfigList[LCD_MAX_STATIC_OBJECTS];
/*!< The static list of handle structures that is returned to the LCD API.*/
static struct lcdHandle_s g_lcdHandleList[LCD_MAX_STATIC_OBJECTS];
/*!< The number of configuration and handle structures created using the LCD API.*/
static uint8_t g_staticConfigsCreated, g_staticHandlesCreated;
#endif


/* waiting macros */
#define Waitns(x) \
        Delay_Waitns(x)                 /* Wait x ns */
#define Waitus(x) \
        Delay_Waitus(x)                 /* Wait x us */
#define Waitms(x) \
        Delay_Waitms(x)                 /* Wait x ms */

/* macros for the RS pin */
#define ClrRS(handle) \
	    MCU_PortClear(handle->config->bus.rs.portRegister, handle->config->bus.rs.pinMask)  /* RS=0: command mode */
#define SetRS(handle) \
	    MCU_PortSet(handle->config->bus.rs.portRegister, handle->config->bus.rs.pinMask)    /* RS=1: data mode */

/* macros for the EN pin */
#define ClrEN(handle) \
	    MCU_PortClear(handle->config->bus.en.portRegister, handle->config->bus.en.pinMask)   /* EN=0 */
#define SetEN(handle) \
	    MCU_PortSet(handle->config->bus.en.portRegister, handle->config->bus.en.pinMask)              /* EN=1 */


#ifndef __FREERTOS_H
#define LcdEnterMutex(x) (void)0
#define LcdExitMutex(x) (void)0
#else
#ifdef LCD_REENTRANT_ACCESS
/* macros for mutex access and release */
#define LcdEnterMutex(x) xSemaphoreTake(x->lcdAccessMutex, portMAX_DELAY)
#define LcdExitMutex(x) xSemaphoreGive(x->lcdAccessMutex)
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/**
 * @brief Creates a pulse to transfer data and/or commands.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void EnablePulse(struct lcdHandle_s* handle);

/**
 * @brief Write a 8 bits value to the LCD data bus.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the 8 bits value.
 *
 */
static void WriteBits(struct lcdHandle_s* handle, uint8_t value);

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(struct lcdHandle_s* handle);

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a kLcdObjectIsHandle, if want to create a LCD handle;
 * 	                   \a kLcdObjectIsConfig, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType);

/**
 * @brief Destroy an specific object used by an LCD instance.
 *
 * @param obj        - The object pointer.
 * @param objectType - \a kLcdObjectIsHandle, if want to destroy a LCD handle;
 * 	                   \a kLcdObjectIsConfig, if want to destroy a LCD configuration structure.
 *
 */
static void DestroyObject(void* obj, uint8_t objectType);


/*******************************************************************************
 * Code
 ******************************************************************************/

static void* CreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case kLcdObjectIsHandle:
		if(g_staticHandlesCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdHandleList[g_staticHandlesCreated++];
		}
		break;
	case kLcdObjectIsConfig:
		if(g_staticConfigsCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	objectCreated = embUtil_Malloc(sizeof(lcdConfig_t));
#endif
	return objectCreated;
}

/*
static void DestroyObject(void* obj, uint8_t objectType)
{
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case kLcdObjectIsHandle:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case kLcdObjectIsConfig:
		if(g_staticConfigsCreated)
			--g_staticConfigsCreated;
		break;
	}
#else
	embUtil_Free(obj);
#endif
	obj = NULL;
}
*/

lcdConfig_t* LCD_CreateConfig(void)
{
	return CreateObject(kLcdObjectIsConfig);
}


static void EnablePulse(struct lcdHandle_s* handle)
{
	ClrEN(handle);
	Waitus(1);
	SetEN(handle);
	Waitus(1);    // enable pulse must be >450ns
	ClrEN(handle);
	Waitus(100);   // commands need > 37us to settle
}


static void SetRowOffsets(struct lcdHandle_s* handle)
{
	handle->row_offsets[0] = 0x00;
	handle->row_offsets[1] = 0x40;
	handle->row_offsets[2] = 0x00 + handle->config->cols;
	handle->row_offsets[3] = 0x40 + handle->config->cols;
}


lcdHandle_t LCD_Init(lcdConfig_t *config)
{
	EmbUtil_Assert(config);

	struct lcdHandle_s* handle = (struct lcdHandle_s*)CreateObject(kLcdObjectIsHandle);
	if(!handle)
	{
		return NULL;
#ifdef __FREERTOS_H
		handle->lcdAccessMutex = xSemaphoreCreateMutex();
		if(!handle->lcdAccessMutex)
		{
			DestroyObject(handle, kLcdObjectIsHandle);
		}
#endif /* __FREERTOS_H */
	}
	handle->config = config;
#ifdef LCD_4BITMODE
	handle->displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
#else
	handle->displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
#endif

    if (config->lines > 1)
    {
    	handle->displayfunction |= LCD_2LINE;
    }

    SetRowOffsets(handle);

    // for some 1 line displays you can select a 10 pixel high font
    if ((config->charsize != LCD_5x8DOTS) && (config->lines == 1)) {
    	handle->displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. So we'll wait 50ms.
    Waitms(50);
    ClrRS(handle);
    ClrEN(handle);

    //put the LCD into 4 bit or 8 bit mode
#ifdef LCD_4BITMODE
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46
    // we start in 8bit mode, try to set 4 bit mode
    WriteBits(handle, 0x03);
    Waitus(4500); // wait min 4.1ms
    // second try
    WriteBits(handle, 0x03);
    Waitus(4500); // wait min 4.1ms
    // third go!
    WriteBits(handle, 0x03);
    Waitus(150);
    // finally, set to 4-bit interface
    WriteBits(handle, 0x02);
#else
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    LCD_Command((lcdHandle_t)handle, LCD_FUNCTIONSET | displayfunction);
    Waitus(4500); // wait min 4.1ms

    // second try
    LCD_Command((lcdHandle_t)handle, LCD_FUNCTIONSET | displayfunction);
    Waitus(150);

    // third go
    LCD_Command((lcdHandle_t)handle, LCD_FUNCTIONSET | displayfunction);
#endif
    // finally, set # lines, font size, etc.
    LCD_Command((lcdHandle_t)handle, LCD_FUNCTIONSET | handle->displayfunction);

    // turn the display on with no cursor or blinking default
    handle->displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_Display((lcdHandle_t)handle);

    // clear it off
    LCD_Clear((lcdHandle_t)handle);

    // Initialize to default text direction (for romance languages)
    handle->displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    LCD_Command((lcdHandle_t)handle, LCD_ENTRYMODESET | handle->displaymode);

    Waitus(400);

    return (lcdHandle_t)handle;
}


/********** high level commands, for the user! */
void LCD_Clear(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	Waitus(2000);  // this command takes a long time!

	LcdExitMutex(handle);
}

void LCD_Home(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_RETURNHOME);  // set cursor position to zero
	Waitus(2000);  // this command takes a long time!

	LcdExitMutex(handle);
}

void LCD_SetCursor(lcdHandle_t handle, uint8_t col, uint8_t row)
{
	EmbUtil_Assert(handle);

	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;

	const size_t max_lines = sizeof(lcdHandle->row_offsets) / sizeof(*lcdHandle->row_offsets);
	if (row >= max_lines)
	{
	row = max_lines - 1;    // we count rows starting w/0
	}
	if (row >= lcdHandle->config->lines)
	{
	row = lcdHandle->config->lines - 1;    // we count rows starting w/0
	}

	LCD_Command(handle, LCD_SETDDRAMADDR | (col + lcdHandle->row_offsets[row]));

	LcdExitMutex(handle);
}


// Turn the display on/off (quickly)
void LCD_NoDisplay(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol &= ~LCD_DISPLAYON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

void LCD_Display(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol |= LCD_DISPLAYON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

// Turns the underline cursor on/off
void LCD_NoCursor(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol &= ~LCD_CURSORON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

void LCD_Cursor(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol |= LCD_CURSORON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

// Turn on and off the blinking cursor
void LCD_NoBlink(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol &= ~LCD_BLINKON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

void LCD_Blink(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaycontrol |= LCD_BLINKON;
	LCD_Command(handle, LCD_DISPLAYCONTROL | lcdHandle->displaycontrol);

	LcdExitMutex(handle);
}

// These commands scroll the display without changing the RAM
void LCD_ScrollDisplayLeft(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);

	LcdExitMutex(handle);
}

void LCD_ScrollDisplayRight(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	LCD_Command(handle, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);

	LcdExitMutex(handle);
}

// This is for text that flows Left to Right
void LCD_LeftToRight(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaymode |= LCD_ENTRYLEFT;
	LCD_Command(handle, LCD_ENTRYMODESET | lcdHandle->displaymode);

	LcdExitMutex(handle);
}

// This is for text that flows Right to Left
void LCD_RightToLeft(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaymode &= ~LCD_ENTRYLEFT;
	LCD_Command(handle, LCD_ENTRYMODESET | lcdHandle->displaymode);

	LcdExitMutex(handle);
}

// This will 'right justify' text from the cursor
void LCD_Autoscroll(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCD_Command(handle, LCD_ENTRYMODESET | lcdHandle->displaymode);

	LcdExitMutex(handle);
}

// This will 'left justify' text from the cursor
void LCD_NoAutoscroll(lcdHandle_t handle)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	lcdHandle->displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_Command(handle, LCD_ENTRYMODESET | lcdHandle->displaymode);

	LcdExitMutex(handle);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_CreateChar(lcdHandle_t handle, uint8_t location, uint8_t charmap[])
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	location &= 0x7; // we only have 8 locations 0-7
	LCD_Command(handle, LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++)
	{
		LCD_Write(handle, charmap[i]);
	}

	LcdExitMutex(handle);
}

void LCD_WriteString(lcdHandle_t handle, char *str)
{
	EmbUtil_Assert(handle);
	LcdEnterMutex(handle);

	while (*str != '\0')
	{
		LCD_Write(handle, *str);
		str++;
	}

	LcdExitMutex(handle);
}

void LCD_CreateBigNumsChars(lcdHandle_t handle)
{
	uint8_t i, j = 0;

	for(i = 0; i < 8; ++i)
	{
		LCD_CreateChar(handle, i, &_bigNumsCodes[j]);
		j += 8;
	}
}

void LCD_WriteBigNum(lcdHandle_t handle, uint8_t col, uint8_t num)
{
	LCD_SetCursor(handle, col, 0);
	LCD_Write(handle, _bigNumCommands[num][0]);
	LCD_Write(handle, _bigNumCommands[num][1]);
	LCD_SetCursor(handle, col, 1);
	LCD_Write(handle, _bigNumCommands[num][2]);
	LCD_Write(handle, _bigNumCommands[num][3]);
}

/*********** mid level commands, for sending data/cmds */

inline void LCD_Command(lcdHandle_t handle, uint8_t value)
{
	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	ClrRS(lcdHandle);
	//Waitus(200);
#ifdef LCD_8BITMODE
    WriteBits(lcdHandle, value);
#else
    WriteBits(lcdHandle, value >> 4);
    WriteBits(lcdHandle, value);
#endif
}

inline void LCD_Write(lcdHandle_t handle, uint8_t value)
{
	struct lcdHandle_s* lcdHandle = (struct lcdHandle_s*)handle;
	SetRS(lcdHandle);
	//Waitus(200);
#ifdef LCD_8BITMODE
    WriteBits(lcdHandle, value);
#else
    WriteBits(lcdHandle, value >> 4);
    WriteBits(lcdHandle, value);
#endif
}


/************ low level data pushing commands **********/

static void WriteBits(struct lcdHandle_s* handle, uint8_t value)
{
#ifdef LCD_4BITMODE
	uint8_t count = 4;
#else
	uint8_t count = 8;
#endif
  for (int i = 0; i < count; i++)
  {
	  if((value >> i) & 0x01)
	  {
		  MCU_PortSet(handle->config->bus.data[i].portRegister, handle->config->bus.data[i].pinMask);
	  }
	  else
	  {
		  MCU_PortClear(handle->config->bus.data[i].portRegister, handle->config->bus.data[i].pinMask);
	  }
  }

  EnablePulse(handle);
}
