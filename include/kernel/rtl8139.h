#ifndef MEME_RTL8139
#define MEME_RTL8139

#include "types.h"

#define RTL8139_NO_PCI_DEV_FOUND 1
#define RTL8139_NO_IO_BAR_FOUND 2

///////////////////////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     rtl8139_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 on success or valid error code

COMMENTS) NONE
******************************************************************************/
u8 rtl8139_init();

#endif