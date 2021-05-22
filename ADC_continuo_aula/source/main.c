/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#define MY_ADC_CHANNEL_GROUP 0U
#define MY_ADC_CHANNEL 0U /*PTE20, ADC0_SE0 */

/*******************************************************************************
 * Code
 ******************************************************************************/
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

    /* Resultados da conversão serão impressos em console de depuração. */
    PRINTF("\r\nADC16 continuos Example.\r\n");

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
    adc16ConfigStruct.enableContinuousConversion = true;
#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif    
    ADC16_Init(ADC0, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC0, false); /* Garante que o disparo de software está sendo utilizado. */

    /* Realiza a calibração do do ADC. */
    if(ADC16_DoAutoCalibration(ADC0) == kStatus_Success)
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
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;

    ADC16_SetChannelConfig(ADC0, MY_ADC_CHANNEL_GROUP, &adc16ChannelConfigStruct);

    while(true)
    {
    	//GETCHAR();GETCHAR();

        // Espere pelo fim da conversão e depois pegue o valor de 12 bits.
        while (kADC16_ChannelConversionDoneFlag != ADC16_GetChannelStatusFlags(ADC0, MY_ADC_CHANNEL_GROUP))
        {
        }

        PRINTF("ADC Value: %d\r\n", ADC16_GetChannelConversionValue(ADC0, MY_ADC_CHANNEL_GROUP));
    }
}
