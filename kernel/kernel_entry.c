// Entry point for the kernel
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "serial.h"
#include "ps2.h"
#include "vga.h"
#include "timer.h"
#include "log.h"
#include "pci.h"
#include "rtl8139.h"
#include "console.h"

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

void kernel_main(void) 
{
    // turn off interrupts while system is brought up
    irq_off();
    
    // Do serial init first
    serial_init_output();

    // Set up core i.e. GDT and IDT
    gdt_install();
    irq_init();

    // Set up drivers
    serial_init_input();
    ps2_init();
    vga_init();
    timer_init();
    pci_init();
    rtl8139_init();

    // Turn interrupts on
    irq_on();

    // Call Console
    console_entry();

    log_msg(__FILE__,__LINE__,"ERROR Should not be here");
}