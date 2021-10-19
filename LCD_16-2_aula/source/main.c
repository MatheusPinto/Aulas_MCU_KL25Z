/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    rt_robot.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
/* TODO: insert other include files here. */
#include "libraries/delay/delay.h"
#include "libraries/emb_util/emb_util.h"
#include "generic_drivers/lcd/lcd.h"

/* TODO: insert other definitions and declarations here. */
const uint8_t myChar[] =
{
		0b00100,
		0b00100,
		0b01010,
		0b01010,
		0b10001,
		0b11111,
		0b00000,
		0b00000,
		0b00000
};

#define LCD_DELTA_CHAR 0x00

/*
 * Para rodar um exemplo, descomente sua macro correspondente e
 * deixe comentado as macros dos outros exemplos.
 *
 * */
//#define EXEMPLO_1
//#define EXEMPLO_2
//#define EXEMPLO_3
//#define EXEMPLO_4

/*
 * @brief   Application entry point.
 */
int main(void)
{
	lcdConfig_t* myLcdConfig;
	lcdHandle_t myLcdHandle;

	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    Delay_Init();

    myLcdConfig = LCD_CreateConfig();

    myLcdConfig->lines = 2;
    myLcdConfig->cols = 16;
    myLcdConfig->charsize = LCD_5x8DOTS;
    myLcdConfig->bus.data[0].portRegister = BOARD_LCD_D4_GPIO;
    myLcdConfig->bus.data[0].pinMask =  BOARD_LCD_D4_PIN_MASK;
    myLcdConfig->bus.data[1].portRegister = BOARD_LCD_D5_GPIO;
    myLcdConfig->bus.data[1].pinMask =  BOARD_LCD_D5_PIN_MASK;
    myLcdConfig->bus.data[2].portRegister = BOARD_LCD_D6_GPIO;
    myLcdConfig->bus.data[2].pinMask =  BOARD_LCD_D6_PIN_MASK;
    myLcdConfig->bus.data[3].portRegister = BOARD_LCD_D7_GPIO;
    myLcdConfig->bus.data[3].pinMask =  BOARD_LCD_D7_PIN_MASK;
    myLcdConfig->bus.rs.portRegister = BOARD_LCD_RS_GPIO;
    myLcdConfig->bus.rs.pinMask =  BOARD_LCD_RS_PIN_MASK;
    myLcdConfig->bus.en.portRegister = BOARD_LCD_EN_GPIO;
    myLcdConfig->bus.en.pinMask =  BOARD_LCD_EN_PIN_MASK;


    myLcdHandle = LCD_Init(myLcdConfig);


    //==========================EXEMPLO_1=================================
#ifdef EXEMPLO_1
    LCD_WriteString(myLcdHandle, "Ola mundo");

    for(;;)
    {
    	LCD_ScrollDisplayLeft(myLcdHandle);
    	Delay_Waitms(500);
    }
#endif
    //====================================================================


    //==========================EXEMPLO_2=================================
#ifdef EXEMPLO_2
    for(;;)
    {
    	LCD_Write(myLcdHandle, 'A');
    	Delay_Waitms(500);
    }
#endif
    //====================================================================

    //==========================EXEMPLO_3=================================
#ifdef EXEMPLO_3
    LCD_CreateChar(myLcdHandle, 0, myChar);

    LCD_SetCursor(myLcdHandle, 0, 0);

    for(;;)
    {
    	LCD_Write(myLcdHandle, LCD_DELTA_CHAR);
    	Delay_Waitms(500);
    }
#endif
    //====================================================================

    //==========================EXEMPLO_4=================================
#ifdef EXEMPLO_4
    LCD_CreateBigNumsChars(myLcdHandle);

    LCD_WriteBigNum(myLcdHandle, 0, 0);
    LCD_WriteBigNum(myLcdHandle, 2, 1);
    LCD_WriteBigNum(myLcdHandle, 5, 2);
    LCD_WriteBigNum(myLcdHandle, 8, 3);
    LCD_WriteBigNum(myLcdHandle, 11, 4);
    LCD_WriteBigNum(myLcdHandle, 14, 5);

    for(;;)
    {
    	__asm("nop");
    }
#endif
    //====================================================================

    return 0 ;
}
