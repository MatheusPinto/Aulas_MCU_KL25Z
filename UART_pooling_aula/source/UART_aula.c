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
 * @file    UART_pooling_aula.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_lpsci.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define BUFFER_LENGHT 100

/*
 * @brief   Application entry point.
 */
int main(void) {

	lpsci_config_t uart0_config;
	int i;
	char buffer[BUFFER_LENGHT];

	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    //BOARD_InitDebugConsole();

    LPSCI_GetDefaultConfig(&uart0_config);
    uart0_config.baudRate_Bps = 115200U;
    uart0_config.enableTx = true;
    uart0_config.enableRx = true;

    /*UART clock:
     * 0 Clock desabilitado
     * 1 MCGFLLCLK clock ou MCGPLLCLK/2 clock
     * 2 OSCERCLK clock
     * 3 MCGIRCLK clock
     * */
    CLOCK_SetLpsci0Clock(1);

    /*Inicializa módulo UART0*/
    LPSCI_Init(UART0, &uart0_config, CLOCK_GetPllFllSelClkFreq());

    const char *bb = "ola mundo\n";
    for(i = 0; i < sizeof("ola mundo\n"); ++i)
    {
    	LPSCI_WriteByte(UART0, bb[i]);
    	while(!(LPSCI_GetStatusFlags(UART0) & UART0_S1_TC_MASK));
    }

    i = 0;
    for(;;)
    {
    	if(LPSCI_GetStatusFlags(UART0) & UART0_S1_RDRF_MASK)
    	{
        	/*Lê bytes um à um até chegar em fim de string ('\n')*/
        	buffer[i] = LPSCI_ReadByte(UART0);

        	if(buffer[i] == '\n')
        	{
        		/*Reenvia a string que foi recebida (echo)*/
        		for(i = 0; buffer[i] != '\n'; ++i)
        		{
        			LPSCI_WriteByte(UART0, buffer[i]);
        			while(!(LPSCI_GetStatusFlags(UART0) & UART0_S1_TC_MASK));
        		}
        		LPSCI_WriteByte(UART0, buffer[i]);
        		while(!(LPSCI_GetStatusFlags(UART0) & UART0_S1_TC_MASK));
        		i = 0; /*Zera contador para leitura da próxima string no buffer*/
        	}
        	else
        		++i;
    	}
    }

    return 0;
}
