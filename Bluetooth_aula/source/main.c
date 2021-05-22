/*
 * Copyright 2016-2021 NXP
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
 * @file    Motor_shield_aula.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_tpm.h"
#include "delayer.h"
#include "stdbool.h"

/* TODO: insert other definitions and declarations here. */
#define BT_MAX_DATA_LENGHT 40
//#define BT_AT_CONFIGURE

/*
 * @brief   Application entry point.
 */
int main(void) {

	char bt_data[BT_MAX_DATA_LENGHT];

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals(); // Inicialização dos periféricos definidos aqui
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

#ifdef BT_AT_CONFIGURE
    PRINTF("Digite os comandos AT ao modulo bluetooth e aguarde as respostas.\n");

    for(;;)
    {
    	SCANF("%s", &bt_data);
    	for(int i = 0; ;++i)
    	{
    		if(bt_data[i] == '\0')
    		{
    			bt_data[i] = '\r';
    			bt_data[i+1] = '\n';
    			break;
    		}
    	}

    	bTooth_SendAT(bt_data);
    	bTooth_ReceiveAT(bt_data);

    	for(int i = 0; ;++i)
    	{
    		if(bt_data[i] == '\n')
    		{
    			bt_data[++i] = '\0';
    			break;
    		}
    	}

    	PRINTF("Resposta: %s\n", bt_data);
    }
#else
    PRINTF("Digite no terminal serial do PC, os comandos para serem imprimidos no console do MCUXpresso via Bluetooth.\n");

    for(;;)
    {
    	size_t i = 0;
    	bTooth_Receive(&bt_data[0], 1);
    	while((i < BT_MAX_DATA_LENGHT-1) && (bt_data[i] != '\r'))
    	{
    		bTooth_Receive(&bt_data[++i], 1);
    	}

   		for(i = 0; bt_data[i] != '\r'; ++i)
   		{
   			PUTCHAR(bt_data[i]);
   		}
   		PUTCHAR(bt_data[i]);
    }

#endif

    return 0 ;
}
