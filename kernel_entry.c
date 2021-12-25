// Entry point for the kernel
#include "core/gdt.h"
#include "core/types.h"
#include "core/irq.h"
#include "drivers/serial.h"
#include "drivers/ps2.h"
#include "drivers/vga.h"
#include "apps/console.h"

void kernel_main(void) 
{
    // Set up core i.e. GDT and IDT
    gdt_install();
    irq_init();

    // Set up drivers
    serial_init();
    ps2_init();
    vga_init();
    timer_init();

    // Turn interrupts on to activate system
    irq_on();

    // Shouldn't get here
}