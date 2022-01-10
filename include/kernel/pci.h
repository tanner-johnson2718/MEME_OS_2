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

#define PCI_DEV_NOT_FOUND 1

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
    u32 bar0_size;
    u8  bar0_prefetch;
    u8  bar0_is_io;
    u8  bar0_is_64b;

    u32 bar1_addr;
    u32 bar1_size;
    u8  bar1_prefetch;
    u8  bar1_is_io;
    u8  bar1_is_64b;

    u32 bar2_addr;
    u32 bar2_size;
    u8  bar2_prefetch;
    u8  bar2_is_io;
    u8  bar2_is_64b;

    u32 bar3_addr;
    u32 bar3_size;
    u8  bar3_prefetch;
    u8  bar3_is_io;
    u8  bar3_is_64b;

    u32 bar4_addr;
    u32 bar4_size;
    u8  bar4_prefetch;
    u8  bar4_is_io;
    u8  bar4_is_64b;

    u32 bar5_addr;
    u32 bar5_size;
    u8  bar5_prefetch;
    u8  bar5_is_io;
    u8  bar5_is_64b;

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

/******************************************************************************
NAME)     pci_get_device_struct

INPUTS)   
          0) u16 venID - vendor ID of the pci struct to return
          1) u16 devID - device ID of the pci struct to return

OUTPUTS)  
          2) dev_ptr - A pointer to a pointer. Will return a pointer to the pci
                       device struct contained within the pci driver.

RETURNS)  0 if successfully found the pci device, error code otherwise

COMMENTS) Returned pci device pointer, points to the single copy of the struct
          in the pci driver memory. A copy is not made.
******************************************************************************/
u8 pci_get_device_struct(u16 venID, u16 devID, struct pci_device** dev_ptr);

#endif