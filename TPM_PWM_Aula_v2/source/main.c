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
 * @file    TPM_PWM_Aula.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"
/* TODO: insert other include files here. */
#include "delay.h"

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

	tpm_config_t tpm2_config;
	tpm_chnl_pwm_signal_param_t chnl_1_param  = {.chnlNumber 		 = kTPM_Chnl_1,
												 .level 	 		 = kTPM_LowTrue,
												 .dutyCycle = 0};
	uint16_t updatedDutycycle = 0;
	bool brightnessUp = true;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    Delay_Init();

    TPM_GetDefaultConfig(&tpm2_config);
    tpm2_config.prescale = kTPM_Prescale_Divide_128;

    TPM_Init (TPM2, &tpm2_config);
    CLOCK_SetTpmClock(1);

    if(TPM_SetupPwm(TPM2,
    		     &chnl_1_param,
       		     1,
     		     kTPM_EdgeAlignedPwm,
     		     24000,
     		     CLOCK_GetPllFllSelClkFreq()) == kStatus_Fail)
    {
    	for(;;);
    }

    TPM_StartTimer(TPM2, kTPM_SystemClock);

    for(;;)
    {
        /* Delays to see the change of LED brightness. */
        Delay_Waitus(50);
        if (brightnessUp)
        {
            /* Increases a duty cycle until it reaches a limited value. */
            if (++updatedDutycycle == 65535U)
            {
                brightnessUp = false;
            }
        }
        else
        {
            /* Decreases a duty cycle until it reaches a limited value. */
            if (--updatedDutycycle == 0U)
            {
                brightnessUp = true;
            }
        }
        /* Starts PWM mode with an updated duty cycle. */
        TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, updatedDutycycle);
    }

    return 0 ;
}
