/*
 * Copyright 2017-2021 NXP
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

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v1.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/**
 * @file    peripherals.c
 * @brief   Peripherals initialization file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include "peripherals.h"

bt_config_t bt_config;
tpm_config_t tpm_pwm_config;
tpm_chnl_pwm_signal_param_t motors_pwm_chnls_param[]  =	{{.chnlNumber 		= TPM_PWM1_CHANNEL,
												 	 	 	  .level 	 		= kTPM_HighTrue,
															  .dutyCyclePercent 	= 0},

													 	 	 {.chnlNumber 		= TPM_PWM2_CHANNEL,
													 	 	  .level 	 		= kTPM_HighTrue,
															  .dutyCyclePercent = 0}};


/**
 * @brief Set up and initialize all required blocks and functions related to the peripherals hardware.
 */
void BOARD_InitBootPeripherals(void) {
	/* The user initialization should be placed here */

	bt_config.baudRate_Bps = 38400U;
	bt_config.uart_base = kBt_UART1;
	bTooth_Init(&bt_config);

	GPIO_SetPinsOutput(IN1_MOTOR_GPIO, IN1_MOTOR_PIN_MASK);
    GPIO_ClearPinsOutput(IN2_MOTOR_GPIO, IN2_MOTOR_PIN_MASK);
    GPIO_ClearPinsOutput(IN3_MOTOR_GPIO, IN3_MOTOR_PIN_MASK);
    GPIO_SetPinsOutput(IN4_MOTOR_GPIO, IN4_MOTOR_PIN_MASK);

    TPM_GetDefaultConfig(&tpm_pwm_config);
    tpm_pwm_config.prescale = kTPM_Prescale_Divide_128;

    TPM_Init (TPM_BASE_PWM, &tpm_pwm_config);
    CLOCK_SetTpmClock(1);

    TPM_SetupPwm(TPM_BASE_PWM,
    			 motors_pwm_chnls_param,
       		     2,
     		     kTPM_EdgeAlignedPwm,
     		     200,
     		     CLOCK_GetPllFllSelClkFreq());

    TPM_StartTimer(TPM_BASE_PWM, kTPM_SystemClock);
}
