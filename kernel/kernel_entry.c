// Entry point for the kernel
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "serial.h"
#include "ps2.h"
#include "vga.h"
#include "timer.h"

#include "sched_driver.h"
#include "sched_app.h"
#include "event.h"
#include "sched.h"

#include "console.h"

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

    // Set up apps
    console_init();

    // Turn interrupts on to activate system
    irq_on();

    while(1)
    {
        sched_thread();
    }
}