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

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u32 pci_get_bar1_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar1(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x14);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u32 pci_get_bar2_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar2(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x18);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u32 pci_get_bar3_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar3(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x1c);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u32 pci_get_bar4_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar4(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x20);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u32 pci_get_bar5_size(u32 bus, u32 device, u32 func)
{
    u32 bar_val = pci_get_bar5(bus, device, func);

    // Now write all 1's to reg
    u32 addr =  pci_build_config_reg(bus, device,func, 0x24);
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, 0xffffffff);

    // write back bar value
    u32  size = (~pci_get_bar_addr(inl(PCI_CONFIG_DATA_PORT))) + 1;
    outl(PCI_CONFIG_ADDR_PORT, addr);
    outl(PCI_CONFIG_DATA_PORT, bar_val);

    return size;
}

u8 pci_get_prefetch(u32 bar_value)
{  
    if(pci_get_mem_or_io(bar_value)) { return 0xff; }   // is IO

    return ((bar_value >> 3) & 0x1);
}

u8 pci_get_64b(u32 bar_value)
{
    if(pci_get_mem_or_io(bar_value)) { return 0xff; }   // is IO

    bar_value = ((bar_value >> 1) & 0x3);

    if(bar_value == 0x0) { return 0; }

    if(bar_value == 0x2) { return 1; }

    log_msg(__FILE__, __LINE__, "Reserved value found in memory mapped BAR");
    return 0xff;
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
    return (u8) pci_read_config(bus, device, func, 0x3c, 8 , 0xff);
}

u8 pci_get_int_line(u32 bus, u32 device, u32 func)
{
    return (u8) pci_read_config(bus, device, func, 0x3c, 0 , 0xff);
}

///////////////////////////////////////////////////////////////////////////////
// Private State
///////////////////////////////////////////////////////////////////////////////

#define PCI_MAX_DEVICES 32
struct pci_device pci_devices[PCI_MAX_DEVICES] = {0};
u8 num_pci_devs = 0;

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

            pci_devices[num_pci_devs].bus = bus;
            pci_devices[num_pci_devs].device = dev;
            pci_devices[num_pci_devs].vendor_ID = pci_get_venID(bus, dev, 0);
            pci_devices[num_pci_devs].device_ID = pci_get_devID(bus, dev, 0);
            pci_devices[num_pci_devs].status = pci_get_stat(bus, dev, 0);
            pci_devices[num_pci_devs].command = pci_get_cmd(bus, dev, 0);
            pci_devices[num_pci_devs].class = pci_get_class(bus, dev, 0);
            pci_devices[num_pci_devs].sub_class = pci_get_subclass(bus, dev, 0);
            pci_devices[num_pci_devs].prog_if = pci_get_progif(bus, dev, 0);
            pci_devices[num_pci_devs].revision_ID = pci_get_revid(bus, dev, 0);
            pci_devices[num_pci_devs].bist = pci_get_bist(bus, dev, 0);
            pci_devices[num_pci_devs].hdr_type = pci_get_hdr_type(bus, dev, 0);
            pci_devices[num_pci_devs].latency_timer = pci_get_latency_timer(bus, dev, 0);
            pci_devices[num_pci_devs].cache_line_size = pci_get_cache_line_size(bus, dev, 0);

            u32 bar0 = pci_get_bar0(bus, dev, 0);
            pci_devices[num_pci_devs].bar0_addr = pci_get_bar_addr(bar0);
            pci_devices[num_pci_devs].bar0_size = pci_get_bar0_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar0_prefetch = pci_get_prefetch(bar0);
            pci_devices[num_pci_devs].bar0_is_io = pci_get_mem_or_io(bar0);
            pci_devices[num_pci_devs].bar0_is_64b = pci_get_64b(bar0);

            u32 bar1 = pci_get_bar1(bus, dev, 0);
            pci_devices[num_pci_devs].bar1_addr = pci_get_bar_addr(bar1);
            pci_devices[num_pci_devs].bar1_size = pci_get_bar1_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar1_prefetch = pci_get_prefetch(bar1);
            pci_devices[num_pci_devs].bar1_is_io = pci_get_mem_or_io(bar1);
            pci_devices[num_pci_devs].bar1_is_64b = pci_get_64b(bar1);

            u32 bar2 = pci_get_bar2(bus, dev, 0);
            pci_devices[num_pci_devs].bar2_addr = pci_get_bar_addr(bar2);
            pci_devices[num_pci_devs].bar2_size = pci_get_bar2_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar2_prefetch = pci_get_prefetch(bar2);
            pci_devices[num_pci_devs].bar2_is_io = pci_get_mem_or_io(bar2);
            pci_devices[num_pci_devs].bar2_is_64b = pci_get_64b(bar2);

            u32 bar3 = pci_get_bar3(bus, dev, 0);
            pci_devices[num_pci_devs].bar3_addr = pci_get_bar_addr(bar3);
            pci_devices[num_pci_devs].bar3_size = pci_get_bar3_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar3_prefetch = pci_get_prefetch(bar3);
            pci_devices[num_pci_devs].bar3_is_io = pci_get_mem_or_io(bar3);
            pci_devices[num_pci_devs].bar3_is_64b = pci_get_64b(bar3);

            u32 bar4 = pci_get_bar4(bus, dev, 0);
            pci_devices[num_pci_devs].bar4_addr = pci_get_bar_addr(bar4);
            pci_devices[num_pci_devs].bar4_size = pci_get_bar4_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar4_prefetch = pci_get_prefetch(bar4);
            pci_devices[num_pci_devs].bar4_is_io = pci_get_mem_or_io(bar4);
            pci_devices[num_pci_devs].bar4_is_64b = pci_get_64b(bar4);

            u32 bar5 = pci_get_bar5(bus, dev, 0);
            pci_devices[num_pci_devs].bar5_addr = pci_get_bar_addr(bar5);
            pci_devices[num_pci_devs].bar5_size = pci_get_bar5_size(bus, dev, 0);
            pci_devices[num_pci_devs].bar5_prefetch = pci_get_prefetch(bar5);
            pci_devices[num_pci_devs].bar5_is_io = pci_get_mem_or_io(bar5);
            pci_devices[num_pci_devs].bar5_is_64b = pci_get_64b(bar5);

            pci_devices[num_pci_devs].cardbus_pointer = pci_get_cardbus_pointer(bus, dev, 0);
            pci_devices[num_pci_devs].subsystem_ID = pci_get_subsystemID(bus, dev, 0);
            pci_devices[num_pci_devs].subsystem_vendor_ID = pci_get_subsystem_vendorID(bus, dev, 0);
            pci_devices[num_pci_devs].expansion_rom_base_addr = pci_get_expansion_rom_addr(bus, dev, 0);
            pci_devices[num_pci_devs].capabilites_pointer = pci_get_capabilities_pointer(bus, dev, 0);
            pci_devices[num_pci_devs].max_latency = pci_get_max_latency(bus, dev, 0);
            pci_devices[num_pci_devs].min_grant = pci_get_min_grant(bus, dev, 0);
            pci_devices[num_pci_devs].interrupt_pin = pci_get_int_pin(bus, dev, 0);
            pci_devices[num_pci_devs].interrupt_line = pci_get_int_line(bus, dev, 0);

            /*
            log_msg(__FILE__, __LINE__, "\n\r");
            log_msg(__FILE__, __LINE__, "Found PCI device: ");
            log_val(__FILE__, __LINE__, "   bus       ", (u32) pci_devices[num_pci_devs].bus);
            log_val(__FILE__, __LINE__, "   dev       ", (u32) pci_devices[num_pci_devs].device);
            log_val(__FILE__, __LINE__, "   ven       ", (u32) pci_devices[num_pci_devs].vendor_ID);
            log_val(__FILE__, __LINE__, "   dev       ", (u32) pci_devices[num_pci_devs].device_ID);
            // log_val(__FILE__, __LINE__, "   stat      ", (u32) pci_devices[num_pci_devs].status);
            // log_val(__FILE__, __LINE__, "   cmd       ", (u32) pci_devices[num_pci_devs].command);
            log_val(__FILE__, __LINE__, "   class     ", (u32) pci_devices[num_pci_devs].class);
            log_val(__FILE__, __LINE__, "   sub class ", (u32) pci_devices[num_pci_devs].sub_class);
            // log_val(__FILE__, __LINE__, "   prog if   ", (u32) pci_devices[num_pci_devs].prog_if);
            // log_val(__FILE__, __LINE__, "   rev id    ", (u32) pci_devices[num_pci_devs].revision_ID);
            // log_val(__FILE__, __LINE__, "   bist      ", (u32) pci_devices[num_pci_devs].bist);
            log_val(__FILE__, __LINE__, "   hdr type  ", (u32) pci_devices[num_pci_devs].hdr_type);
            // log_val(__FILE__, __LINE__, "   lat timer ", (u32) pci_devices[num_pci_devs].latency_timer);
            // log_val(__FILE__, __LINE__, "   cache size", (u32) pci_devices[num_pci_devs].cache_line_size);
            
            if(bar0)
            {
                log_val(__FILE__, __LINE__, "   bar0      ", (u32) bar0);
                log_val(__FILE__, __LINE__, "   bar0 addr ", (u32) pci_devices[num_pci_devs].bar0_addr);
                log_val(__FILE__, __LINE__, "   bar0 size ", (u32) pci_devices[num_pci_devs].bar0_size);
                log_val(__FILE__, __LINE__, "   bar0 pre  ", (u32) pci_devices[num_pci_devs].bar0_prefetch);
                log_val(__FILE__, __LINE__, "   bar0 64b  ", (u32) pci_devices[num_pci_devs].bar0_is_64b);
            }

            if(bar1)
            {         
                log_val(__FILE__, __LINE__, "   bar1      ", (u32) bar1);
                log_val(__FILE__, __LINE__, "   bar1 addr ", (u32) pci_devices[num_pci_devs].bar1_addr);
                log_val(__FILE__, __LINE__, "   bar1 size ", (u32) pci_devices[num_pci_devs].bar1_size);
                log_val(__FILE__, __LINE__, "   bar1 pre  ", (u32) pci_devices[num_pci_devs].bar1_prefetch);
                log_val(__FILE__, __LINE__, "   bar1 64b  ", (u32) pci_devices[num_pci_devs].bar1_is_64b);
            }
            
            if(bar2)
            {
                log_val(__FILE__, __LINE__, "   bar2      ", (u32) bar2);
                log_val(__FILE__, __LINE__, "   bar2 addr ", (u32) pci_devices[num_pci_devs].bar2_addr);
                log_val(__FILE__, __LINE__, "   bar2 size ", (u32) pci_devices[num_pci_devs].bar2_size);
                log_val(__FILE__, __LINE__, "   bar2 pre  ", (u32) pci_devices[num_pci_devs].bar2_prefetch);
                log_val(__FILE__, __LINE__, "   bar2 64b  ", (u32) pci_devices[num_pci_devs].bar2_is_64b);
            }
            
            if(bar3)
            {
                log_val(__FILE__, __LINE__, "   bar3      ", (u32) bar3);
                log_val(__FILE__, __LINE__, "   bar3 addr ", (u32) pci_devices[num_pci_devs].bar3_addr);
                log_val(__FILE__, __LINE__, "   bar3 size ", (u32) pci_devices[num_pci_devs].bar3_size);
                log_val(__FILE__, __LINE__, "   bar3 pre  ", (u32) pci_devices[num_pci_devs].bar3_prefetch);
                log_val(__FILE__, __LINE__, "   bar3 64b  ", (u32) pci_devices[num_pci_devs].bar3_is_64b);
            }
            
            if(bar4)
            {
                log_val(__FILE__, __LINE__, "   bar4      ", (u32) bar4);
                log_val(__FILE__, __LINE__, "   bar4 addr ", (u32) pci_devices[num_pci_devs].bar4_addr);
                log_val(__FILE__, __LINE__, "   bar4 size ", (u32) pci_devices[num_pci_devs].bar4_size);
                log_val(__FILE__, __LINE__, "   bar4 pre  ", (u32) pci_devices[num_pci_devs].bar4_prefetch);
                log_val(__FILE__, __LINE__, "   bar4 64b  ", (u32) pci_devices[num_pci_devs].bar4_is_64b);
            }
            
            if(bar5)
            {
                log_val(__FILE__, __LINE__, "   bar5      ", (u32) bar5);
                log_val(__FILE__, __LINE__, "   bar5 addr ", (u32) pci_devices[num_pci_devs].bar5_addr);
                log_val(__FILE__, __LINE__, "   bar5 size ", (u32) pci_devices[num_pci_devs].bar5_size);
                log_val(__FILE__, __LINE__, "   bar5 pre  ", (u32) pci_devices[num_pci_devs].bar5_prefetch);
                log_val(__FILE__, __LINE__, "   bar5 64b  ", (u32) pci_devices[num_pci_devs].bar5_is_64b);
            }
            
            log_val(__FILE__, __LINE__, "   cis ptr   ", (u32) pci_devices[num_pci_devs].cardbus_pointer);
            // log_val(__FILE__, __LINE__, "   subsys ID ", (u32) pci_devices[num_pci_devs].subsystem_ID);
            // log_val(__FILE__, __LINE__, "   subsysven ", (u32) pci_devices[num_pci_devs].subsystem_vendor_ID);
            log_val(__FILE__, __LINE__, "   expansion ", (u32) pci_devices[num_pci_devs].expansion_rom_base_addr);
            log_val(__FILE__, __LINE__, "   cap ptr   ", (u32) pci_devices[num_pci_devs].capabilites_pointer);
            // log_val(__FILE__, __LINE__, "   max lat   ", (u32) pci_devices[num_pci_devs].max_latency);
            // log_val(__FILE__, __LINE__, "   min grant ", (u32) pci_devices[num_pci_devs].min_grant);
            log_val(__FILE__, __LINE__, "   intr pin  ", (u32) pci_devices[num_pci_devs].interrupt_pin);
            log_val(__FILE__, __LINE__, "   intr line ", (u32) pci_devices[num_pci_devs].interrupt_line);
            */
           
            ++num_pci_devs;
        }
    }

    return 0;
}

/******************************************************************************
NAME)     pci_get_device_struct

INPUTS)   
          0) u8 venID - vendor ID of the pci struct to return
          1) u8 devID - device ID of the pci struct to return

OUTPUTS)  
          2) dev_ptr - A pointer to a pointer. Will return a pointer to the pci
                       device struct contained within the pci driver.

RETURNS)  0 if successfully found the pci device, error code otherwise

COMMENTS) Returned pci device pointer, points to the single copy of the struct
          in the pci driver memory. A copy is not made.
******************************************************************************/
u8 pci_get_device_struct(u16 venID, u16 devID, struct pci_device** dev_ptr)
{
    u8 i;
    for(; i < num_pci_devs; ++i)
    {
        if(pci_devices[i].vendor_ID == venID && pci_devices[i].device_ID == devID)
        {
            *dev_ptr = &pci_devices[i];
            return 0;
        }
    }

    log_msg(__FILE__, __LINE__, "PCI get device failed to find device");
    log_val(__FILE__, __LINE__, "venID", venID);
    log_val(__FILE__, __LINE__, "devID", devID);
    return PCI_DEV_NOT_FOUND;
}