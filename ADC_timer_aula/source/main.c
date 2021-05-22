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
 * @file    ADC_timer_aula.c
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

/* TODO: insert other definitions and declarations here. */
#define MY_ADC_GROUP 0U
#define MY_ADC_CHANNEL 0U /*PTE20, ADC0_SE0 */

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_AdcConversionDoneFlag = false;
volatile uint32_t g_AdcConversionValue;
volatile uint32_t g_AdcInterruptCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/

void ADC0_IRQHandler(void)
{
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC16_GetChannelConversionValue(ADC0, MY_ADC_GROUP);
    if((++g_AdcInterruptCounter)%4 == 0)
    {
    	g_AdcConversionDoneFlag = true;
    }
}

/*
 * ADC0 trigger select
Selects the ADC0 trigger source when alternative triggers are functional in stop and VLPS modes. .
0x0 External trigger pin input (EXTRG_IN)
0x1 CMP0 output
0x4 PIT trigger 0
0x5 PIT trigger 1
0x8 TPM0 overflow
0x9 TPM1 overflow
0xA TPM2 overflow
0xC RTC alarm
0xD RTC seconds
0xE LPTMR0 trigger*/
void BOARD_ConfigADCTriggerSource(uint8_t hwTrigger)
{
    /* Configure SIM for ADC hw trigger source selection */
    SIM->SOPT7 |= (0x00000080U | hwTrigger);
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;

    tpm_config_t tpm0_config;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_ConfigADCTriggerSource(0x8); // Selecionado TPM0 como fonte de gatilho do ADC
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Resultados da conversão serão impressos em console de depuração. */
    PRINTF("\r\nADC16 timer Example.\r\n");

    EnableIRQ(ADC0_IRQn); /* Habilita interrupção pelo NVIC. */

    TPM_GetDefaultConfig(&tpm0_config);
    tpm0_config.prescale = kTPM_Prescale_Divide_128;
    TPM_Init(TPM0, &tpm0_config);
    CLOCK_SetTpmClock(1);
    TPM_SetTimerPeriod(TPM0, 0xFFFFU);
    TPM_EnableInterrupts(TPM0, kTPM_TimeOverflowInterruptEnable);

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC0, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC0, true); /* Garante que o disparo de hardware está sendo utilizado. */

    /* Realiza a calibração do do ADC. */
    if (kStatus_Success == ADC16_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    adc16ChannelConfigStruct.channelNumber = MY_ADC_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Habilita interrupção. */
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
    ADC16_SetChannelConfig(ADC0, MY_ADC_GROUP, &adc16ChannelConfigStruct);

    TPM_StartTimer(TPM0, kTPM_SystemClock);

    while(true)
    {
    	/* Espere pelo fim da conversão quando "ADC0_IRQHandler" irá
    	   setar g_AdcConversionDoneFlag*/
    	while (!g_AdcConversionDoneFlag)
    	{
    	}
        g_AdcConversionDoneFlag = false;

        PRINTF("ADC Value: %d\t\t", g_AdcConversionValue);
        PRINTF("ADC Interrupt Count: %d\r\n", g_AdcInterruptCounter);
    }

    return 0 ;
}
