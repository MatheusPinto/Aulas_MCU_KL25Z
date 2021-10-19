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
 *
 *
 * O módulo Whatchdog do Kinetis é denominado COP ("Computer Operation Properly").
 * Nesse exemplo, faremos um LED da placa piscar na frequência definida pelo
 * usuário no console da IDE. O Whatchdog será ressetado sempre no fim do período
 * do LED piscar. Se deixarmos a frequência de piscar o LED muito baixa, então
 * o Watchdog irá ressetar o microcontrolador.
 *
 * Nota: No arquivo CMSIS/system_MKL25Z4.c, excluir a macro DISABLE_WDOG para
 * poder usar o WDOG. Como a habilitação/desabilitação só pode ser feita uma
 * vez a cada vez que o MCU é ligado, permitir ao sistema desabilitar na
 * inicialização (em SystemInit), não deixará possível habilitar o WGOD após
 * o main.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_cop.h"
#include "fsl_rcm.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "delay.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    cop_config_t configCop;
    int ledPeriod;

    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    Delay_Init();

    /* A fonte do clock do WDOG/COP pode ser:
     *  - O barramento dos periféricos (BUS) ou;
     *  - O oscilador de 1kHz, o LDO (quando queremos mais baixas frequências). */

    /* O período do WDOG é limitado por 4 combinações:
     *                                         LPO  ou BUS
     * kCOP_2Power5CyclesOr2Power13Cycles  --  2^5  ou 2^13 clock cycles
     * kCOP_2Power8CyclesOr2Power16Cycles  --  2^8  ou 2^16 clock cycles
     * kCOP_2Power10CyclesOr2Power18Cycles --  2^10 ou 2^18 clock cycles
     *
     * Nesse exemplo, iremos utilizar o LPO de 1kHz e
     * período = kCOP_2Power10CyclesOr2Power18Cycles ==  2^10 ciclos.
     * Ou seja, o período do WDOG será 1,024 segundos.
     * */

    /*
     * configCop.enableWindowMode = false;           --> Nunca usei essa opção!
     * configCop.timeoutMode = kCOP_LongTimeoutMode; --> Não sei porque tem essa opção já que definimos o período pelos clocks e ciclos...
     * configCop.enableStop = false;
     * configCop.enableDebug = false;
     * configCop.clockSource = kCOP_LpoClock;
     * configCop.timeoutCycles = kCOP_2Power10CyclesOr2Power18Cycles;
     */
    COP_GetDefaultConfig(&configCop);
    configCop.timeoutCycles = kCOP_2Power10CyclesOr2Power18Cycles;

    /* Novamente, não sei porque tem essa opção já que definimos o período pelos clocks e ciclos...*/
#if FSL_FEATURE_COP_HAS_LONGTIME_MODE
    configCop.timeoutMode = kCOP_ShortTimeoutMode;
#endif

    /* Verifica se o WDOG ressetou o MCU na última vez. */
    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        PRINTF("Reset due to COP timeout\r\n");
    }

    PRINTF("\r\nExemplo WDOG aula!\r\n");
    PRINTF("\t-Digite um valor inteiro com o periodo de piscar o LED (em ms): ");
    SCANF("%d", &ledPeriod);
    COP_Init(SIM, &configCop);

    while (true)
    {
    	COP_Refresh(SIM); /* Resseta o WDOG */
    	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, BOARD_INITPINS_LED_GREEN_PIN_MASK);
    	Delay_Waitms(ledPeriod);
    }
}
