/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc16.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "stdbool.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
    g_AdcConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC16_GetChannelConversionValue(ADC0, MY_ADC_GROUP);
    g_AdcInterruptCounter++;
}

/*!
 * @brief Main function
 */
int main(void)
{
    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    EnableIRQ(ADC0_IRQn); /* Habilita interrupção pelo NVIC. */

    /* Resultados da conversão serão impressos em console de depuração. */
    PRINTF("\r\nADC16 interrupt Example.\r\n");

    /*
     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.enableAsynchronousClock = true;
     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
     * adc16ConfigStruct.enableHighSpeed = false;
     * adc16ConfigStruct.enableLowPower = false;
     * adc16ConfigStruct.enableContinuousConversion = false;
     */
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
    	/* Calibração pode falhar devido a diversos fatores:
		 * ver manual do KL25Z, pag. 473, bit CALF.
		*/
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    PRINTF("Press ENTER each time to get a ADC value ...\r\n");

    adc16ChannelConfigStruct.channelNumber = MY_ADC_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Habilita interrupção. */
    adc16ChannelConfigStruct.enableDifferentialConversion = false;

    g_AdcInterruptCounter = 0U;

    while(true)
    {
        GETCHAR();GETCHAR();
        g_AdcConversionDoneFlag = false;
        /*
         * Quando estiver no modo de disparo por software, cada conversão deve ser feita chamando
         * a função "ADC16_SetChannelConfig()", que irá enviar o comando de conversão e
         * executar a conversão. Para uma conversão em outro canal, basta mudar o campo "channelNumber"
         * na estrutura de configuração do canal e chamar novamente o "ADC16_SetChannelConfig()".
         * Note que em conversões disparadas por software, "MY_ADC_GROUP" será sempre 0.
         */
        ADC16_SetChannelConfig(ADC0, MY_ADC_GROUP, &adc16ChannelConfigStruct);

        /* Espere pelo fim da conversão quando "ADC0_IRQHandler" irá
        setar adcConversionDoneFlag*/
        while (!g_AdcConversionDoneFlag)
        {
        }
        PRINTF("ADC Value: %d\r\n", g_AdcConversionValue);
        PRINTF("ADC Interrupt Count: %d\r\n", g_AdcInterruptCounter);
    }
}
