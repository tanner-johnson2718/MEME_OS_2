// Entry point for the kernel
#include "core/gdt.h"
#include "core/types.h"
#include "core/irq.h"
#include "drivers/serial.h"
#include "drivers/ps2.h"
#include "drivers/vga.h"
#include "drivers/timer.h"
#include "apps/console.h"

#include "sched/driver_api.h"
#include "sched/app_api.h"

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

    while(1)
    {
        
    }
}