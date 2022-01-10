#include "rtl8139.h"
#include "types.h"
#include "io_port.h"
#include "pci.h"
#include "log.h"
#include "irq.h"

///////////////////////////////////////////////////////////////////////////////
// Private
///////////////////////////////////////////////////////////////////////////////

struct pci_device* rtl8139_device = 0;
u32 rtl8139_io_addr = 0;
u8 rtl8139_pic_irq = 0xff;

#define RTL8139_CMD_REG 0x37
#define RTL8139_CONFIG1 0x52

void rtl8139_power_on()
{
    if(rtl8139_io_addr)
    {
        outb(rtl8139_io_addr + RTL8139_CONFIG1, 0x0);
        log_msg(__FILE__, __LINE__, "RTL8139 powered on");
    }  
}

void rtl8139_sw_reset()
{
    if(rtl8139_io_addr)
    {
        outb( rtl8139_io_addr + RTL8139_CMD_REG, 0x10);
        while( (inb(rtl8139_io_addr + RTL8139_CMD_REG) & 0x10) != 0) { }
        log_msg(__FILE__, __LINE__, "RTL8139 reset");
    }  
}

void rtl8139_irq_handler()
{

}

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
u8 rtl8139_init()
{
    // first grab PCI device
    u8 ret = pci_get_device_struct(0x10EC, 0x8139, &rtl8139_device);
    if(ret || !rtl8139_device)
    {
        log_msg(__FILE__, __LINE__, "RTL8139 failed to grab pci device");
        return RTL8139_NO_PCI_DEV_FOUND;
    }

    log_val(__FILE__, __LINE__, "RTL8139 bus ", rtl8139_device->bus);
    log_val(__FILE__, __LINE__, "RTL8139 dev ", rtl8139_device->device);

    // search through bars to find io_addr
    if(rtl8139_device->bar0_addr && rtl8139_device->bar0_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar0_addr;
    }
    if(rtl8139_device->bar1_addr && rtl8139_device->bar1_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar1_addr;
    }
    if(rtl8139_device->bar2_addr && rtl8139_device->bar2_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar2_addr;
    }
    if(rtl8139_device->bar3_addr && rtl8139_device->bar3_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar3_addr;
    }
    if(rtl8139_device->bar4_addr && rtl8139_device->bar4_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar4_addr;
    }
    if(rtl8139_device->bar5_addr && rtl8139_device->bar5_is_io)
    {
        if(rtl8139_io_addr)
        {
            log_msg(__FILE__,__LINE__, "Warning found multiple IO addrs for RTL8139");
        }
        rtl8139_io_addr = rtl8139_device->bar5_addr;
    }

    if(!rtl8139_io_addr)
    {
        return RTL8139_NO_IO_BAR_FOUND;
    }

    log_val(__FILE__, __LINE__, "RTL8139 IO addr ", rtl8139_io_addr);

    // Parse irq line
    rtl8139_pic_irq = rtl8139_device->interrupt_line;
    log_val(__FILE__, __LINE__, "RTL8139 IRQ Line ", rtl8139_pic_irq);

    // Set irq handler

    // power on

    // reset

    // Init recv buffer

    // set IMR + ISR

    // Config recv buffer

    // enable TX/RX

}