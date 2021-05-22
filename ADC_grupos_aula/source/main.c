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
 * @file    ADC_grupos_aula.c
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
#define ADC_GROUP_A 0U
#define ADC_GROUP_B 1U
#define ADC_CHANNEL_A 0U /*PTE20, ADC0_SE0 */
#define ADC_CHANNEL_B 4U /*PTE21, ADC0_SE4a */

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_AdcTimerConversionDoneFlag = false;
volatile uint32_t g_AdcTimerConversionValue;
volatile uint32_t g_AdcInterruptCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/

void ADC0_IRQHandler(void)
{
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcTimerConversionValue = ADC16_GetChannelConversionValue(ADC0, ADC_GROUP_B);
    if((++g_AdcInterruptCounter)%4 == 0)
    {
    	g_AdcTimerConversionDoneFlag = true;
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
    adc16_channel_config_t adcChannelAConfigStruct, adcChannelBConfigStruct;

    tpm_config_t tpm0_config;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_ConfigADCTriggerSource(0x8); // Selecionado TPM0 como fonte de gatilho do ADC
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Resultados da conversão serão impressos em console de depuração. */
    PRINTF("\r\nADC16 two groups Example.\r\n");

    EnableIRQ(ADC0_IRQn); /* Habilita interrupção pelo NVIC. */

    /*Define as configurações do temporizador*/
    TPM_GetDefaultConfig(&tpm0_config);
    tpm0_config.prescale = kTPM_Prescale_Divide_128;
    TPM_Init(TPM0, &tpm0_config);
    CLOCK_SetTpmClock(1);
    TPM_SetTimerPeriod(TPM0, 0xFFFFU);
    TPM_EnableInterrupts(TPM0, kTPM_TimeOverflowInterruptEnable);

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    adc16ConfigStruct.enableContinuousConversion = true; // Funciona apenas no disparo por software (grupo A)
    ADC16_Init(ADC0, &adc16ConfigStruct);

    /* Realiza a calibração do do ADC. */
    if (kStatus_Success == ADC16_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
    	// Calibração pode falhar devido a ...
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    /* No modo continuo de conversão, é preciso disparar apenas uma vez o ADC por software.
     * Dessa forma, defino esse modo de convsersão pertencente ao grupo A, seto os parâmetros e
     * disparo o ADC.*/
    ADC16_EnableHardwareTrigger(ADC0, false); /* Garante que o disparo de software está sendo utilizado. */
    adcChannelAConfigStruct.channelNumber = ADC_CHANNEL_A;
    adcChannelAConfigStruct.enableInterruptOnConversionCompleted = false;
    adcChannelAConfigStruct.enableDifferentialConversion = false;
    ADC16_SetChannelConfig(ADC0, ADC_GROUP_A, &adcChannelAConfigStruct); /* Disparo é feito apenas uma única vez aqui. */

    /* Após disparado o ADC no modo continuo de conversão, posso realizar outra fonte de disparo por hardware
     * definindo a conversão pertecente ao grupo B. Ambas as conversões serão feitas "simultâneamente"
     * (quando uma terminar a outra começa em seguida).
     * Dessa forma, defino esse modo de conversão disparado pelo temporizados TPM0 de forma periódica.*/
    ADC16_EnableHardwareTrigger(ADC0, true); /* Garante que o disparo de hardware está sendo utilizado. */
    adcChannelBConfigStruct.channelNumber = ADC_CHANNEL_B;
    adcChannelBConfigStruct.enableInterruptOnConversionCompleted = true; /* Habilita interrupção. */
    adcChannelBConfigStruct.enableDifferentialConversion = false;
    ADC16_SetChannelConfig(ADC0, ADC_GROUP_B, &adcChannelBConfigStruct); /* Habilita o disparo feito pelo temporizador. */
    TPM_StartTimer(TPM0, kTPM_SystemClock);

    while(true)
    {
    	/* Espere pelo fim da conversão quando "ADC0_IRQHandler" irá
    	   setar g_AdcConversionDoneFlag e a conversão contínua estiver pronta.*/
    	while ((!g_AdcTimerConversionDoneFlag)&&
    			(kADC16_ChannelConversionDoneFlag != ADC16_GetChannelStatusFlags(ADC0, ADC_GROUP_A)))
    	{
    	}
        g_AdcTimerConversionDoneFlag = false;

        // Este é o resultado da conversão disparado pelo temporizador no seu canal especifico.
        PRINTF("ADC Timer Value: %d\t\t", g_AdcTimerConversionValue);
        // Este é o resultado da conversão feito continuamente e disparado uma vez por software no seu canal especifico.
        PRINTF("ADC Continuous Value: %d\t\t", ADC16_GetChannelConversionValue(ADC0, ADC_GROUP_A));
    }

    return 0 ;
}
