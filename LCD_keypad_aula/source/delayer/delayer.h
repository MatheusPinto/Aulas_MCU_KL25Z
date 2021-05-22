#ifndef __delayer_H
#define __delayer_H

/* MODULE delayer. */

/* Include shared modules, which are used for whole project */

/* Include inherited beans */

#include "MKL25Z4.h"
#include "fsl_clock.h"
#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t frequency, cyclesForUs, cyclesForMs;

#define delayer_NofCyclesMs(ms)  ((ms)*cyclesForMs) /* calculates the needed cycles based on bus clock frequency */
#define delayer_NofCyclesUs(us)  ((us)*cyclesForUs) /* calculates the needed cycles based on bus clock frequency */
#define delayer_NofCyclesNs(ns)  ((ns)*cyclesForUs)/1000 /* calculates the needed cycles based on bus clock frequency */


#define delayer_WAIT_C(cycles) \
     ( (cycles)<=10 ? \
          delayer_Wait10Cycles() \
        : delayer_WaitCycles((uint16_t)cycles) \
      )                                      /*!< wait for some cycles */

void delayer_Init(void);

void delayer_Wait10Cycles(void);
/*
** ===================================================================
**     Method      :  delayer_Wait10Cycles (component Wait)
**     Description :
**         Wait for 10 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void delayer_Wait100Cycles(void);
/*
** ===================================================================
**     Method      :  delayer_Wait100Cycles (component Wait)
**     Description :
**         Wait for 100 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void delayer_WaitCycles(uint16_t cycles);
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

void delayer_Waitms(uint16_t ms);
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

/* we are having a static clock configuration: implement as macro/inlined version */
#define delayer_Waitus(us) delayer_WAIT_C(delayer_NofCyclesUs(us))

/*
#define delayer_Waitus(us)  \
       (  ((delayer_NofCyclesUs((us),frequency)==0)||(us)==0) ? \
          (void)0 : \
          ( ((us)/1000)==0 ? (void)0 : delayer_Waitms((uint16_t)((us)/1000))) \
          , (delayer_NofCyclesUs(((us)%1000), frequency)==0) ? (void)0 : \
            delayer_WAIT_C(delayer_NofCyclesUs(((us)%1000), frequency)) \
       )
*/

/*
#define delayer_Waitus(us)  \
       (  ((delayer_NofCyclesUs((us),frequency)==0)||(us)==0) ? \
          (void)0 : \
          ( ((us)/1000)==0 ? (void)0 : delayer_Waitms((uint16_t)((us)/1000))) \
          , (delayer_NofCyclesUs(((us)%1000), CLOCK_GetCoreSysClkFreq())==0) ? (void)0 : \
            delayer_WAIT_C(delayer_NofCyclesUs(((us)%1000), CLOCK_GetCoreSysClkFreq())) \
       )
*/
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

/* we are having a static clock configuration: implement as macro/inlined version */
#define delayer_Waitns(ns) delayer_WAIT_C(delayer_NofCyclesNs(ns))

/*
#define delayer_Waitns(ns)  \
       (  ((delayer_NofCyclesNs((ns))==0)||(ns)==0) ? \
          (void)0 : \
          delayer_Waitus((ns)/1000) \
          , (delayer_NofCyclesNs((ns)%1000)==0) ? \
              (void)0 : \
              delayer_WAIT_C(delayer_NofCyclesNs(((ns)%1000))) \
       )
*/
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

#define delayer_WaitOSms(ms) \
  delayer_Waitms(ms) /* no RTOS used, so use normal wait */
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

#ifdef __cplusplus
}  /* extern "C" */
#endif

void delayer_WaitLongCycles(uint32_t cycles);
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

/* END delayer. */

#endif
/* ifndef __delayer_H */

