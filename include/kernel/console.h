#ifndef MEME_CONSOLE
#define MEME_CONSOLE

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* Console application that acts as the main interface between the user and the
   system. Can read memory, write memory, examine registers, and call driver 
   functions.
*/

///////////////////////////////////////////////////////////////////////////////
// Console API
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     console_entry

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 always succeeds

COMMENTS) NONE
******************************************************************************/
u8 console_entry();

#endif