// Entry point for the kernel
#include "core/gdt.h"
#include "core/types.h"
#include "core/irq.h"
#include "drivers/serial.h"
#include "drivers/ps2.h"
#include "drivers/vga.h"
#include "drivers/timer.h"
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

    u32 time_prev = 0;
    u32 i = 0;
    while(1)
    {
        u32 time = timer_get_time_ms();
        
        if( (time % 1000 == 0) && (time != time_prev) )
        {
            vga_textmode_putc(i, 0, 'a', VGA_BLUE, VGA_RED);
            ++i;
        }

        time_prev = time;
        
    }
}