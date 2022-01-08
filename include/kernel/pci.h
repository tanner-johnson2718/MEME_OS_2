#ifndef MEME_PCI
#define MEME_PCI

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* Big oof.... todo
*/

///////////////////////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////////////////////

struct pci_device       // assumes header type 0
{
    u8 bus;
    u8 device;
    u8 function;
    u16 device_ID;
    u16 vendor_ID;
    u16 status;
    u16 command;
    u8 class;
    u8 sub_class;
    u8 prog_if;
    u8 revision_ID;
    u8 bist;
    u8 hdr_type;
    u8 latency_timer;
    u8 cache_line_size;
    u32 bar0_addr;
    u32 bar1_addr;
    u32 bar2_addr;
    u32 bar3_addr;
    u32 bar4_addr;
    u32 bar5_addr;
    u32 bar0_size;
    u32 bar1_size;
    u32 bar2_size;
    u32 bar3_size;
    u32 bar4_size;
    u32 bar5_size;
    u32 cardbus_pointer;
    u16 subsystem_ID;
    u16 subsystem_vendor_ID;
    u32 expansion_rom_base_addr;
    u8 capabilites_pointer;
    u8 max_latency;
    u8 min_grant;
    u8 interrupt_pin;
    u8 interrupt_line;
} __attribute__((packed));

/******************************************************************************
NAME)     pci_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 pci_init();

#endif