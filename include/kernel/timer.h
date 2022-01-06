#ifndef MEME_TIMER
#define MEME_TIMER

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* The time driver will interface with the 8253/8254 PIT and will initialize to
   simply generate a square wave at 1Khz, or 1000 times every second. The timer
   can thus keep track of CPU time by simlply counting this timer and inc. a 
   millisecond CPU time internal value. This keeps the driver simple and this 
   functionality can be changed at a later time to do fancier things, for now
   its good. The driver registers an IRQ handler for the corresponding PIC 
   interrupt.

   For more doc see "docs/Programmable Interval Timer - OSDev Wiki.html"
*/

#define TIMER_FAIL_REG_IRQ 1

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     timer_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 timer_init();


/******************************************************************************
NAME)     timer_get_time_ms

INPUTS)   
          0) time - pointer to 32bit value to hold time

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 timer_get_time_ms(u32* time);

#endif