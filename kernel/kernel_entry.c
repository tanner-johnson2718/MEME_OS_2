// Entry point for the kernel
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "serial.h"
#include "ps2.h"
#include "vga.h"
#include "timer.h"

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

    // Turn interrupts on
    irq_on();

    // Some test code
    vga_textmode_clear_screen(VGA_WHITE, VGA_BLACK);
    vga_textmode_putc(0, 0, 'h', VGA_WHITE, VGA_BLACK);
    vga_textmode_putc(1, 0, 'e', VGA_WHITE, VGA_BLACK);
    vga_textmode_putc(2, 0, 'l', VGA_WHITE, VGA_BLACK);
    vga_textmode_putc(3, 0, 'l', VGA_WHITE, VGA_BLACK);
    vga_textmode_putc(4, 0, 'o', VGA_WHITE, VGA_BLACK);

    while(1)
    {
        
    }
}