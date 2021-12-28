// Entry point for the kernel
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "serial.h"
#include "ps2.h"
#include "vga.h"
#include "timer.h"

#include "driver_api.h"
#include "app_api.h"

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

    u8 data[4] = {0xff, 0xff,0xff,0xff};
    sched_dump_event_buffers();
    serial_puts("------------------------------");
    sched_driver_publish_IN_event(data, 4, 0);
    sched_driver_publish_IN_event(data, 4, 1);
    sched_driver_publish_IN_event(data, 4, 2);
    sched_driver_publish_IN_event(data, 4, 3);
    sched_dump_event_buffers();
    serial_puts("------------------------------");

    sched_app_pop_IN_event(1, data, 4);
    sched_app_pop_IN_event(2, data, 4);
    sched_dump_event_buffers();
    serial_puts("------------------------------");

    sched_app_pop_IN_event(0, data, 4);
    sched_dump_event_buffers();

    while(1)
    {
        
    }
}