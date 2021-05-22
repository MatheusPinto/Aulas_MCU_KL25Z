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
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
tpm_config_t tpm0_config;
const gpio_pin_config_t ptb19_config = {kGPIO_DigitalOutput, 1};

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    GPIO_PinInit(GPIOB, 19, &ptb19_config);

    TPM_GetDefaultConfig(&tpm0_config);
    tpm0_config.prescale = kTPM_Prescale_Divide_128;
    TPM_Init (TPM0, &tpm0_config);
    CLOCK_SetTpmClock(1);

    TPM_SetTimerPeriod(TPM0, 0xBFFFU);
    TPM_EnableInterrupts (TPM0, kTPM_TimeOverflowInterruptEnable);
    NVIC_EnableIRQ(TPM0_IRQn);
    TPM_StartTimer(TPM0, kTPM_SystemClock);

    for(;;);

    return 0;
}

void TPM0_IRQHandler(void)
{
    if((TPM_GetStatusFlags(TPM0) & kTPM_TimeOverflowFlag) != 0)
    {
        GPIO_TogglePinsOutput(GPIOB, 1 << 19);
    }

    TPM_ClearStatusFlags(TPM0, kTPM_TimeOverflowFlag);
}

