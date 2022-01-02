#ifndef MEME_GDT
#define MEME_GDT

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

// The Global descriptor table (GDT) was created at first to expand the amount
// of memory you could access using 16 bits. It was extended to add memory
// protection. Now in days it exsist mainly for backwards compatibilty. We will
// use a "flat" memory model where the kernel and user code and data segments
// simply cover the entire 4Gb address space and this mechanism is pretty much
// by passed.

// The GDT defines memory regions along with permisions adn extra meta data
// describing how this segment is used. The registers CS,DS, and SS to keep 
// indexes (offsets) into the GDT. You use the gdt entry format to store 
// entires in this table, and use the lgdt, sgdt to set and get the tables 
// location.

// See docs/BKD_GDT.htm and docs/GDT_GDT.html for more details.

///////////////////////////////////////////////////////////////////////////////
// GDT Public MACROS and defines
///////////////////////////////////////////////////////////////////////////////

#define GDT_NULL_SECTOR 0x0
#define GDT_KERNEL_CODE_SECTOR 0x8
#define GDT_KERNEL_DATA_SECTOR 0x10
#define GDT_USER_CODE_SECTOR 0x8
#define GDT_USER_DATA_SECTOR 0x10

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     gdt_install

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always a success

COMMENTS) NONE
******************************************************************************/
u8 gdt_install();

#endif