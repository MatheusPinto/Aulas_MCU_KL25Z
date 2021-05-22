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
 * @file    TPM_ultrassonic_aula.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "stdbool.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_tpm.h"
#include "delayer/delayer.h"

/* TODO: insert other definitions and declarations here. */
bool isFallingEdgeWaiting, ultraIsCaptured;
uint16_t ultraEchoPulseCount;

#define TPM_TIMER_PERIOD     0xFFFFU

void TPM1_IRQHandler(void)
{
	if(TPM_GetStatusFlags(TPM1) & kTPM_Chnl0Flag)
	{
		TPM_ClearStatusFlags(TPM1, kTPM_Chnl0Flag);
		/* Na ocorrência de cada borda de echo, o valor do contador
		 * é salvo em TPM0->CONTROLS[0].CnV.
		 * Se for borda de descida...*/
		if(isFallingEdgeWaiting)
		{
			/* ...obtém a largura do pulso em números de contagens.*/
			if(TPM1->CONTROLS[0].CnV > ultraEchoPulseCount)
			{
				ultraEchoPulseCount = TPM1->CONTROLS[0].CnV - ultraEchoPulseCount;
			}
			else
			{
				/* Se ocorreu overflow de contagem, o valor atual do contador será
				 * menor que o valor salvo anteriormente. Dessa forma, a equação abaixo
				 * ajusta para que o valor absoluto entre esses tempos seja obtido.*/
				ultraEchoPulseCount = TPM1->CONTROLS[0].CnV + (TPM_TIMER_PERIOD - ultraEchoPulseCount);
			}
			ultraIsCaptured = true;
			isFallingEdgeWaiting = false;
		}
		else
		{
			/* ...Se for borda de subida, apenas salva o valor de TPM0->CONTROLS[0].CnV.*/
			isFallingEdgeWaiting = true;
			ultraEchoPulseCount = TPM1->CONTROLS[0].CnV;
		}
	}
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
	tpm_config_t tpm1_config;
    uint16_t d; // distância dos objetos medidos
	uint32_t tpm_timerResolution; // resolução do temporizador in ns

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    TPM_GetDefaultConfig(&tpm1_config);
    tpm1_config.prescale = kTPM_Prescale_Divide_128;
    TPM_Init (TPM1, &tpm1_config);

    CLOCK_SetTpmClock(1);
    TPM_SetTimerPeriod(TPM1, TPM_TIMER_PERIOD);

    /* O modo captura de entrada é um modo de configuração do temporizador,
     * assim como o PWM. Definimos o canal e a forma de interrupção no
     * pino do canal. Então quando uma interrupção é gerada no canal,
     * a ISR do TPM é chamada e o valor do contador na hora da interrupção
     * é salvo automaticamente no registrador TPMx->CONTROLS[n].CnV,
     * onde n é o numero do canal.
     * Esse valor pode ser salvo em uma variável na chamada da ISR.*/
    TPM_SetupInputCapture(TPM1, kTPM_Chnl_0, kTPM_RiseAndFallEdge); // PTA12

    TPM_EnableInterrupts(TPM1, kTPM_Chnl0InterruptEnable);
    NVIC_EnableIRQ(TPM1_IRQn);

    TPM_StartTimer(TPM1, kTPM_SystemClock);

    /* Esperar por ruidos de echo do ultrassonico gerados por causa
     * da inicilização do pino de trigger. */
    delayer_Waitms(500);
    isFallingEdgeWaiting = false;
    ultraIsCaptured = false;

    tpm_timerResolution = 1000000000U/(CLOCK_GetPllFllSelClkFreq()/128U);

    PRINTF("TPM using ultrassonic example:\n");
    PRINTF("\t-TPM clock freq: %u Hz\n", CLOCK_GetPllFllSelClkFreq()/128U);
    PRINTF("\t-TPM clock resolution:%u (ns)\n", tpm_timerResolution);

    while(true)
    {
    	/*Gera pulso de trigger - PTB9*/
    	GPIO_SetPinsOutput(BOARD_INITPINS_TRIGGER_GPIO, BOARD_INITPINS_TRIGGER_PIN_MASK);
    	delayer_Waitus(10);
    	GPIO_ClearPinsOutput(BOARD_INITPINS_TRIGGER_GPIO, BOARD_INITPINS_TRIGGER_PIN_MASK);

    	/*Espera pulso de echo ser capturado...*/
    	while(!ultraIsCaptured);
    	ultraIsCaptured = false;

    	/*Calcula a distância do objeto em cm e imprime no console.*/
    	d = (1715U*((ultraEchoPulseCount*tpm_timerResolution)/1000U))/100000U;
    	PRINTF("Object distance: %u cm\n", d);
    	delayer_Waitms(400);
    }

    return 0 ;
}
