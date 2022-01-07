#include "pci.h"
#include "types.h"
#include "io_port.h"
#include "log.h"
#include "serial.h"

///////////////////////////////////////////////////////////////////////////////
// Private defines, macros, and registers (Configuration Space)
///////////////////////////////////////////////////////////////////////////////

// Registers for accessing the configuration space, write address to addr port
// and read from the data port. On failure,returns 0xFFFF in the vendor ID.
// Each PCI device defines a 256 byte conifguration space. Reads into this space
// are defined above and must be aligned to 4 bytes.
#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#define PCI_CONFIG_REG_SHIFT 0
#define PCI_CONFIG_FUNC_SHIFT 8
#define PCI_CONFIG_DEV_SHIFT 11
#define PCI_CONFIG_BUS_SHIFT 16
#define PCI_CONFIG_ENABLE    0x80000000

#define PCI_DEVID_OFFSET 0x0
#define PCI_DEVID_SHIFT 16
#define PCI_DEVID_MASK 0xffff

u16 pci_get_devID(u32 bus, u32 device, u32 func)
{
    bus = bus << PCI_CONFIG_BUS_SHIFT;
    device = device << PCI_CONFIG_DEV_SHIFT;
    func = func << PCI_CONFIG_FUNC_SHIFT;

    u32 addr =  PCI_CONFIG_ENABLE |  PCI_DEVID_OFFSET;
    addr    |= bus;
    addr    |= device;
    addr    |= func;

    outl(PCI_CONFIG_ADDR_PORT, addr);
    u32 data = inl(PCI_CONFIG_DATA_PORT);

    return ((data >> PCI_DEVID_SHIFT) & PCI_DEVID_MASK);
}

#define PCI_VENID_OFFSET 0x0
#define PCI_VENID_SHIFT 0
#define PCI_VENID_MASK 0xffff

u16 pci_get_venID(u32 bus, u32 device, u32 func)
{
    bus = bus << PCI_CONFIG_BUS_SHIFT;
    device = device << PCI_CONFIG_DEV_SHIFT;
    func = func << PCI_CONFIG_FUNC_SHIFT;

    u32 addr =  PCI_CONFIG_ENABLE |  PCI_VENID_OFFSET;
    addr    |= bus;
    addr    |= device;
    addr    |= func;

    outl(PCI_CONFIG_ADDR_PORT, addr);
    u32 data = inl(PCI_CONFIG_DATA_PORT);

    return ((data >> PCI_VENID_SHIFT) & PCI_VENID_MASK);
}

///////////////////////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     pci_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 pci_init()
{
    u32 bus = 0;
    u32 dev = 0;

    for(bus = 0; bus < 256; ++bus)
    {
        for(dev = 0; dev < 32; ++dev)
        {
            u16 venID = pci_get_venID(bus, dev, 0);
            
            if(venID == 0xffff) { continue; }

            log_msg(__FILE__, __LINE__, "Found PCI device: ");
            log_val(__FILE__, __LINE__, "bus", bus);
            log_val(__FILE__, __LINE__, "dev", dev);
            log_val(__FILE__, __LINE__, "ven", (u32) venID);
            
        }
    }
}