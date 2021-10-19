/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
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
 * @file    Timer_aula.c
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
#include "delay.h"
/* TODO: insert other include files here. */
//#define SET_TO_ZERO_DEGREE


/* TODO: insert other definitions and declarations here. */
tpm_config_t tpm0_config;
tpm_chnl_pwm_signal_param_t chnl_1_param = {kTPM_Chnl_1, kTPM_HighTrue, 0};

/*
 * @brief   Application entry point.
 */
int main(void) {

	tpm_config_t tpm2_config;
	tpm_chnl_pwm_signal_param_t chnl_0_param  =	{.chnlNumber 		= kTPM_Chnl_0,
												 .level 	 		= kTPM_HighTrue,
												 .dutyCyclePercent 	= 0};
	/* Valor inicial para ser enviado ao servo.
	 * Angulo de -90 graus == 2,5% de 65535*/
	uint16_t currentDutyCycle = 1638;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    Delay_Init();

    TPM_GetDefaultConfig(&tpm2_config);
    tpm2_config.prescale = kTPM_Prescale_Divide_128;

    TPM_Init (TPM2, &tpm2_config);
    CLOCK_SetTpmClock(1);

    /* Frequência do PWM para o servo == 50 Hz.
     * Período de contagem de 20 ms corresponde à 65535 (limite máximo do registrador contador).
     * Pulsos variam entre 0,5 ms (-90 graus, 2,5% de 20 ms) e 2,4 ms (+90 graus, 12% de 20 ms).
     * */
    TPM_SetupPwm(TPM2,
    		    &chnl_0_param ,
       		    1,
     		    kTPM_EdgeAlignedPwm,
     		    50,
     		    CLOCK_GetPllFllSelClkFreq());

    TPM_StartTimer(TPM2, kTPM_SystemClock);

#ifdef SET_TO_ZERO_DEGREE
    TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_0, kTPM_EdgeAlignedPwm, currentDutyCycle);
#endif
    for(;;)
    {
#ifndef SET_TO_ZERO_DEGREE
    	/* Servo irá iniciar no ângulo mínimo de -90 graus.
    	 * Ciclo de trabalho irá aumentar aos poucos à cada iteração.
    	 * Irá sair do laço quando alcançar ângulo máximo:
    	 * +90 graus == 12% de 65535 == 7864.*/
    	while(currentDutyCycle < 7864)
    	{
    		currentDutyCycle += 100;
    		TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_0, kTPM_EdgeAlignedPwm, currentDutyCycle);
    		Delay_Waitms(20);
    	}

    	/* Servo irá iniciar no ângulo máximo de +90 graus.
    	 * Ciclo de trabalho irá diminuir aos poucos à cada iteração.
    	 * Irá sair do laço quando alcançar ângulo mínimo:
    	 * -90 graus == 2,5% de 65535 == 1638.*/
    	while(currentDutyCycle > 1638)
    	{
    		currentDutyCycle -= 100;
     		TPM_UpdatePwmDutycycle(TPM2, kTPM_Chnl_0, kTPM_EdgeAlignedPwm, currentDutyCycle);
     		Delay_Waitms(20);
        }
#else

#endif
    }

    return 0;
}


