#ifndef MEME_PS2
#define MEME_PS2

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* PS2 keyboard driver will interface with the PS2 controller and handler key
   board input. It will keep track of caps like, shift held, etc. and return
   only the appropiate ascii char to a handler register with this driver.

   For more doc on PS2 see "docs/PS_2 Keyboard - OSDev Wiki.html".
*/

#define PS2_IRQ_REG_FAILED 1

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     ps2_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 on success, otherwise error code

COMMENTS) NONE
******************************************************************************/
u8 ps2_init(void);



/******************************************************************************
NAME)     ps2_register_handler

INPUTS)   
          0) handler - function pointer that takes ascii byte as input

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 ps2_register_hanlder(void (*handler)(u8));

#endif