/**
 * @file	delay.c
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2021
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A library with delay generation functions.
 *
 * This library was adapted from Processor Expert Component by Erich Styger:
 * https://sourceforge.net/projects/mcuoneclipse/files/
 *
 * Supported CPUs:
 *
 *   - ARM-Cortex M0 to M7;
 *   - RISC-V
 *
 * Supported OSes:
 *
 *   - FreeRTOS.
 *
 */

#include "delay.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */

/*!< DWT Control register */
#define DELAY_ARM_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))

/*!< CYCCNTENA bit in DWT_CONTROL register */
#define DELAY_ARM_DWT_CYCCNTENA_BIT       (1UL<<0)

/*!< DWT Cycle Counter register */
#define DELAY_ARM_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))

/*!< DEMCR: Debug Exception and Monitor Control Register */
#define DELAY_ARM_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))

/*!< Trace enable bit in DEMCR register */
#define DELAY_ARM_TRCENA_BIT              (1UL<<24)

uint32_t g_mcuCoreFrequency;
uint32_t g_mcuCyclesForUs, g_mcuCyclesForMs;

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/**
 * @brief Wait for 10 CPU cycles.
 *
 */
void Wait10Cycles(void);

/**
 * @brief Wait for 100 CPU cycles.
 *
 */
void Wait100Cycles(void);


/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register.
 *
 */
#define ArmInitCycleCounter() \
  DELAY_ARM_DEMCR |= DELAY_ARM_TRCENA_BIT

/**
 * @brief Reset cycle counter.
 *
 */
#define ArmResetCycleCounter() \
  DELAY_ARM_DWT_CYCCNT = 0

/**
 * @brief Enable cycle counter.
 *
 */
#define ArmEnableCycleCounter() \
  DELAY_ARM_DWT_CONTROL |= DELAY_ARM_DWT_CYCCNTENA_BIT

/**
 * @brief Disable cycle counter.
 *
 */
#define ArmDisableCycleCounter() \
  DELAY_ARM_DWT_CONTROL &= ~DELAY_ARM_DWT_CYCCNTENA_BIT

/**
 * @brief Read cycle counter register.
 *
 */
#define ArmGetCycleCounter() \
  DELAY_ARM_DWT_CYCCNT

void Delay_Init(void)
{
#ifdef DELAY_USE_ARM_CYCLE_COUNTER
    /* init cycle counter */
    ArmInitCycleCounter();
    ArmResetCycleCounter();
    ArmEnableCycleCounter();
#endif
	g_mcuCoreFrequency = DELAY_CLOCK_FREQUENCY;
	g_mcuCyclesForMs = (g_mcuCoreFrequency/1000);
	g_mcuCyclesForUs = (g_mcuCoreFrequency/1000)/1000;
}

#ifdef __GNUC__
#ifdef DELAY_CPU_IS_RISC_V /* naked is ignored for RISC-V gcc */
  #ifdef __cplusplus  /* gcc 4.7.3 in C++ mode does not like no_instrument_function: error: can't set 'no_instrument_function' attribute after definition */
  #else
    __attribute__((no_instrument_function))
  #endif
#else
  #ifdef __cplusplus  /* gcc 4.7.3 in C++ mode does not like no_instrument_function: error: can't set 'no_instrument_function' attribute after definition */
    __attribute__((naked))
  #else
    __attribute__((naked, no_instrument_function))
  #endif
#endif
#endif
void Wait10Cycles(void)
{
    /* This function will wait 10 CPU cycles (including call overhead). */
    /*lint -save -e522 function lacks side effect. */

#ifdef DELAY_CPU_IS_ARM_CORTEX_M
    /* NOTE: Cortex-M0 and M4 have 1 cycle for a NOP */
    /* Compiler is GNUC */
    __asm (
    /* bl Wait10Cycles() to here: [4] */
    "nop   \n\t" /* [1] */
    "nop   \n\t" /* [1] */
    "nop   \n\t" /* [1] */
    "bx lr \n\t" /* [3] */
    );
#elif DELAY_CPU_IS_RISC_V
  /* \todo */
    __asm ( /* assuming [4] for overhead */
    "nop   \n\t" /* [1] */
    "nop   \n\t" /* [1] */
    "nop   \n\t" /* [1] */
    "nop   \n\t" /* [1] */
  );
#endif
  /*lint -restore */
}

#ifdef __GNUC__
  #ifdef DELAY_CPU_IS_RISC_V /* naked is ignored for RISC-V gcc */
    #ifdef __cplusplus  /* gcc 4.7.3 in C++ mode does not like no_instrument_function: error: can't set 'no_instrument_function' attribute after definition */
    #else
      __attribute__((no_instrument_function))
    #endif
  #else
    #ifdef __cplusplus  /* gcc 4.7.3 in C++ mode does not like no_instrument_function: error: can't set 'no_instrument_function' attribute after definition */
      __attribute__((naked))
    #else
      __attribute__((naked, no_instrument_function))
    #endif
  #endif
#endif
void Wait100Cycles(void)
{
   /* This function will spend 100 CPU cycles (including call overhead). */
   /*lint -save -e522 function lacks side effect. */
#ifdef DELAY_CPU_IS_ARM_CORTEX_M
   __asm (
     /* bl to here:               [4] */
     "push {r0}   \n\t"        /* [2] */
     "movs r0, #0 \n\t"        /* [1] */
     "loop:       \n\t"
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "add r0,#1   \n\t"        /* [1] */
     "cmp r0,#9   \n\t"        /* [1] */
     "bls loop    \n\t"        /* [3] taken, [1] not taken */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "nop         \n\t"        /* [1] */
     "pop {r0}    \n\t"        /* [2] */
     "bx lr       \n\t"        /* [3] */
   );
#elif DELAY_CPU_IS_RISC_V
   /* \todo */
   __asm ( /* assuming [10] for overhead */
	 "  li a5,20        \n\t"
	 "Loop:             \n\t"
	 "  addi a5,a5,-1   \n\t"
	 "  bgtz a5, Loop   \n\t"
   );
#endif
  /*lint -restore */
}

void Delay_WaitCycles(uint32_t cycles)
{
/*lint -save -e522 function lacks side effect. */
#ifdef DELAY_USE_ARM_CYCLE_COUNTER

   cycles += ArmGetCycleCounter();
   while (ArmGetCycleCounter() < cycles)
   {
	 /* wait */
   }
#else

   while (cycles > 1000)
   {
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 Wait100Cycles();
	 cycles -= 1000;
   }
   /*If cycles > 100 conditional evaluation will be minimized
     calling delayer_Wait100Cycles. */
   while (cycles > 100)
   {
	 Wait100Cycles();
	 cycles -= 100;
   }
   while (cycles > 10)
   {
	 Wait10Cycles();
	 cycles -= 10;
   }
#endif
  /*lint -restore */
}

void Delay_Waitms(uint16_t ms)
{
   /*lint -save -e522 function lacks side effect. */
   while (ms > 0)
   {
     Delay_WaitCycles(g_mcuCyclesForMs);
     --ms;
   }
  /*lint -restore */
}
