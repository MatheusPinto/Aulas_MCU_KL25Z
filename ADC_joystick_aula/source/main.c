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
 * @file    ADC_joystick_aula.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_tpm.h"
#include "fsl_adc16.h"
#include "stdbool.h"
#include "delayer/delayer.h"

/* TODO: insert other definitions and declarations here. */
#define ADC_GROUP_A 0U
#define ADC_CHANNEL 4U

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief   Application entry point.
 */
int main(void) {

    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adcChannel4ConfigStruct;
    char axis;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Resultados da conversão serão impressos em console de depuração. */
    PRINTF("\r\nADC16 four channels Example.\r\n");

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC0, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC0, false); /* Garante que o disparo de software está sendo utilizado. */

    /* Realiza a calibração do do ADC. */
    if (kStatus_Success == ADC16_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
    	PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    adcChannel4ConfigStruct.channelNumber = ADC_CHANNEL;
    adcChannel4ConfigStruct.enableInterruptOnConversionCompleted = false;
    adcChannel4ConfigStruct.enableDifferentialConversion = false;

    while(true)
    {
    	ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxA);
    	axis = 'X';
    	for(int i = 0; i < 2; ++i)
    	{
        	ADC16_SetChannelConfig(ADC0, ADC_GROUP_A, &adcChannel4ConfigStruct);
    		// Espere pelo fim da conversão e depois pegue o valor.
    		while (kADC16_ChannelConversionDoneFlag != ADC16_GetChannelStatusFlags(ADC0, ADC_GROUP_A))
    		{
    		}
    		// O valor pego aqui será de 12 bits: 0 à 4095
    		PRINTF("ADC Value %c: %d\r\n", axis, ADC16_GetChannelConversionValue(ADC0, ADC_GROUP_A));
    		ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxB);
    		axis = 'Y';
    	}
    	PRINTF("\n");
    	delayer_Waitms(300);
    }

    return 0 ;
}
