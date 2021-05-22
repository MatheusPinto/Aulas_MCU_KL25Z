/* MODULE delayer. */

#include "delayer.h"


/*
** ===================================================================
**     Method      :  delayer_Wait10Cycles (component Wait)
**     Description :
**         Wait for 10 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__attribute__((naked)) void delayer_Wait10Cycles(void)
{
  /* This function will wait 10 CPU cycles (including call overhead). */
  /* NOTE: Cortex-M0 and M4 have 1 cycle for a NOP */
  /* Compiler is GNUC */
  __asm (
   /* bl Wai10Cycles() to here: [4] */
   "nop   \n\t" /* [1] */
   "nop   \n\t" /* [1] */
   "nop   \n\t" /* [1] */
   "bx lr \n\t" /* [3] */
  );
}

/*
** ===================================================================
**     Method      :  delayer_Wait100Cycles (component Wait)
**     Description :
**         Wait for 100 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__attribute__((naked)) void delayer_Wait100Cycles(void)
{
  /* This function will spend 100 CPU cycles (including call overhead). */
  __asm (
   /* bl to here:               [4] */
   "movs r0, #0 \n\t"        /* [1] */
   "loop:       \n\t"
   "nop         \n\t"        /* [1] */
   "nop         \n\t"        /* [1] */
   "nop         \n\t"        /* [1] */
   "nop         \n\t"        /* [1] */
   "nop         \n\t"        /* [1] */
   "add r0,#1   \n\t"        /* [1] */
   "cmp r0,#9   \n\t"        /* [1] */
   "bls loop    \n\t"        /* [3] taken, [1] not taken */
   "nop         \n\t"        /* [1] */
   "bx lr       \n\t"        /* [3] */
  );
}

/*
** ===================================================================
**     Method      :  delayer_WaitCycles (component Wait)
**     Description :
**         Wait for a specified number of CPU cycles (16bit data type).
**     Parameters  :
**         NAME            - DESCRIPTION
**         cycles          - The number of cycles to wait.
**     Returns     : Nothing
** ===================================================================
*/
void delayer_WaitCycles(uint16_t cycles)
{
  while(cycles > 100) {
    delayer_Wait100Cycles();
    cycles -= 100;
  }
  while(cycles > 10) {
    delayer_Wait10Cycles();
    cycles -= 10;
  }
}

/*
** ===================================================================
**     Method      :  delayer_WaitLongCycles (component Wait)
**     Description :
**         Wait for a specified number of CPU cycles (32bit data type).
**     Parameters  :
**         NAME            - DESCRIPTION
**         cycles          - The number of cycles to wait.
**     Returns     : Nothing
** ===================================================================
*/
void delayer_WaitLongCycles(uint32_t cycles)
{
  while(cycles>60000) {
    delayer_WaitCycles(60000);
    cycles -= 60000;
  }
  delayer_WaitCycles((uint16_t)cycles);
}

/*
** ===================================================================
**     Method      :  delayer_Waitms (component Wait)
**     Description :
**         Wait for a specified time in milliseconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ms              - How many milliseconds the function has to
**                           wait
**     Returns     : Nothing
** ===================================================================
*/
void delayer_Waitms(uint16_t ms)
{
  uint32_t msCycles; /* cycles for 1 ms */

  /* static clock/speed configuration */
  msCycles = delayer_NofCyclesMs(1, CLOCK_GetCoreSysClkFreq());
  while(ms>0) {
    delayer_WaitLongCycles(msCycles);
    ms--;
  }
}
/*
** ===================================================================
**     Method      :  delayer_Waitus (component Wait)
**     Description :
**         Wait for a specified time in microseconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         us              - How many microseconds the function has to
**                           wait
**     Returns     : Nothing
** ===================================================================
*/
/* implemented as macro version. See header file. */
/*
** ===================================================================
**     Method      :  delayer_Waitns (component Wait)
**     Description :
**         Wait for a specified time in nano seconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ns              - How many ns the function has to wait
**     Returns     : Nothing
** ===================================================================
*/
/* implemented as macro version. See header file. */
/*
** ===================================================================
**     Method      :  delayer_WaitOSms (component Wait)
**     Description :
**         If an RTOS is enabled, this routine will use a non-blocking
**         wait method. Otherwise it will do a busy/blocking wait.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#if 0
void delayer_WaitOSms(void)
{
  /* Method is implemented as macro in the header file */
}
#endif

/* END delayer. */
