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
// are defined above and must be aligned to 4 bytes. ASSUME THE CONFIG SPACE
// USES THE STANDARD HDR ONLY.
#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#define PCI_CONFIG_REG_SHIFT 0
#define PCI_CONFIG_FUNC_SHIFT 8
#define PCI_CONFIG_DEV_SHIFT 11
#define PCI_CONFIG_BUS_SHIFT 16
#define PCI_CONFIG_ENABLE    0x80000000

u32 pci_build_config_reg(u32 bus, u32 device, u32 func, u32 offset)
{
    bus = bus << PCI_CONFIG_BUS_SHIFT;
    device = device << PCI_CONFIG_DEV_SHIFT;
    func = func << PCI_CONFIG_FUNC_SHIFT;

    u32 addr =  PCI_CONFIG_ENABLE |  offset;
    addr    |= bus;
    addr    |= device;
    addr    |= func;

    return addr;
}

u32 pci_read_config(u32 bus, u32 device, u32 func, u32 offset, u32 shift, u32 mask)
{
    u32 addr =  pci_build_config_reg(bus, device,func, offset);

    outl(PCI_CONFIG_ADDR_PORT, addr);
    u32 data = inl(PCI_CONFIG_DATA_PORT);

    return ((data >> shift) & mask);
}

u16 pci_get_devID(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0, 16, 0xffff);
}

u16 pci_get_venID(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0, 0, 0xffff);
}

u16 pci_get_stat(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0x4, 16, 0xffff);
}

u16 pci_get_cmd(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0x4, 0, 0xffff);
}

u8 pci_get_class(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x8, 24, 0xff);
}

u8 pci_get_subclass(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x8, 16, 0xff);
}

u8 pci_get_progif(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x8, 8, 0xff);
}

u8 pci_get_revid(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x8, 0, 0xff);
}

u8 pci_get_bist(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0xc, 24, 0xff);
}

u8 pci_get_hdr_type(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0xc, 16, 0xff);
}

u8 pci_get_latency_timer(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0xc, 8, 0xff);
}

u8 pci_get_cache_line_size(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0xc, 0, 0xff);
}

u32 pci_get_bar0(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x10, 0, 0xffffffff);
}

u32 pci_get_bar1(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x14, 0, 0xffffffff);
}

u32 pci_get_bar2(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x18, 0, 0xffffffff);
}

u32 pci_get_bar3(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x1c, 0, 0xffffffff);
}

u32 pci_get_bar4(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x20, 0, 0xffffffff);
}

u32 pci_get_bar5(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x24, 0, 0xffffffff);
}

// 1 for IO, 0 for mem
u8 pci_get_mem_or_io(u32 bar_val)
{
    return (u8) (bar_val & 0x1);
}

u8 pci_get_bar_type(u32 bar_val)
{
    return (u8) ((bar_val >> 1) & 0x3);
}

u8 pci_get_bar_prefetchable(u32 bar_val)
{
    return (u8) ((bar_val >> 3) & 0x1);
}

u32 pci_get_bar_addr(u32 bar_val)
{
    if(pci_get_mem_or_io(bar_val))
    {
        // IO
        return (bar_val & 0xfffffffc); 

    }
    else
    {
        return (bar_val & 0xfffffff0);
    }
}

u32 pci_get_bar0_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x10);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    return (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
}

u32 pci_get_bar1_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x14);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    return (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
}

u32 pci_get_bar2_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x18);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    return (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
}

u32 pci_get_bar3_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x1c);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    return (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
}

u32 pci_get_bar4_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x20);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    u32 ret = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;

    // return bar value
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

}

u32 pci_get_bar5_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar0(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x24);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);


    return (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
}

u32 pci_get_cardbus_pointer(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x28, 0, 0xffffffff);
}

u16 pci_get_subsystemID(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0x2c, 16, 0xffff);
}

u16 pci_get_subsystem_vendorID(u32 bus, u32 device, u32 func)
{
    return (u16) pci_read_config(bus, device, func, 0x2c, 0, 0xffff);
}

u32 pci_get_expansion_rom_addr(u32 bus, u32 device, u32 func)
{
    return pci_read_config(bus, device, func, 0x30, 0, 0xffffffff);
}

u8 pci_get_capabilities_pointer(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x34, 0 , 0xff);
}

u8 pci_get_max_latency(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x3c, 24 , 0xff);
}

u8 pci_get_min_grant(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x3c, 16 , 0xff);
}

u8 pci_get_int_pin(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x3c, 16 , 0xff);
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
            log_val(__FILE__, __LINE__, "   bus      ", bus);
            log_val(__FILE__, __LINE__, "   dev      ", dev);
            log_val(__FILE__, __LINE__, "   ven      ", (u32) pci_get_venID(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   dev      ", (u32) pci_get_devID(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   stat     ", (u32) pci_get_stat(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   cmd      ", (u32) pci_get_cmd(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   class    ", (u32) pci_get_class(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   sub class", (u32) pci_get_subclass(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   prog if  ", (u32) pci_get_progif(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   rev id   ", (u32) pci_get_revid(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bist     ", (u32) pci_get_bist(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   hdr type  ", (u32) pci_get_hdr_type(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   lat timer ", (u32) pci_get_latency_timer(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   cache size", (u32) pci_get_cache_line_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar0      ", (u32) pci_get_bar0(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar0 size ", (u32) pci_get_bar0_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar1      ", (u32) pci_get_bar1(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar1 size ", (u32) pci_get_bar1_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar2      ", (u32) pci_get_bar2(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar2 size ", (u32) pci_get_bar2_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar3      ", (u32) pci_get_bar3(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar3 size ", (u32) pci_get_bar3_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar4      ", (u32) pci_get_bar4(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar4 size ", (u32) pci_get_bar4_size(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar5      ", (u32) pci_get_bar5(bus, dev, 0));
            log_val(__FILE__, __LINE__, "   bar5 size ", (u32) pci_get_bar5_size(bus, dev, 0));
        }
    }
}