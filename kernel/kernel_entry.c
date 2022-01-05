// Entry point for the kernel
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "serial.h"
#include "ps2.h"
#include "vga.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// Basic Console Code
///////////////////////////////////////////////////////////////////////////////

u8 bg = VGA_BLACK;
u8 fg = VGA_WHITE;
u8 row = 0;
u8 col = 0;

#define MAX_LINE_SIZE 256
u8 line_index = 0;
u8 line_buffer[MAX_LINE_SIZE];

void update_screen_index(u8 c)
{
    col++;
    if(col == VGA_BUFFER_WIDTH)
    {
        row++;
        col = 0;
    }

    if(row == VGA_BUFFER_HEIGHT)
    {
        row = 0;
        col = 0;
    }

    if(c == '\n')
    {
        row++;
        col = 0;
        if(row == VGA_BUFFER_HEIGHT)
        {
            row = 0;
        }
    }

    vga_textmode_update_cursor(col,row);
}

void kb_hanle(u8 in)
{
    if(in != '\n')
    {
        vga_textmode_putc(col, row, in, VGA_WHITE, VGA_BLACK);
    }
    else
    {
        // enter pressed
    }
    
    update_screen_index(in);
}

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

void kernel_main(void) 
{
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

    // Turn interrupts on
    irq_on();

    // Some test code
    vga_textmode_clear_screen(fg, bg);
    vga_textmode_update_cursor(col,row);
    ps2_register_hanlder(kb_hanle);

    while(1)
    {
        // IDLE
    }
}