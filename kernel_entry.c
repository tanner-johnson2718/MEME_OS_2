// Entry point for the kernel
#include "uart_8250.h"
#include "gdt.h"
#include "types.h"
#include "irq.h"

extern u32 kernel_end;

void kernel_main(void) 
{

    serial_init();
    serial_puts("\n\r");

    struct gdt_ptr p;
    gdt_get_curr_gdt_ptr(&p);
    serial_puts("GDT pointer = ");
    serial_put_hex(p.base);
    serial_puts("\n\r");
    serial_puts("GDT Size = ");
    serial_put_hex(p.limit);
    serial_puts("\n\r");

    gdt_install();

    gdt_get_curr_gdt_ptr(&p);
    serial_puts("\n\r");
    serial_puts("GDT pointer = ");
    serial_put_hex(p.base);
    serial_puts("\n\r");
    serial_puts("GDT Size = ");
    serial_put_hex(p.limit);
    serial_puts("\n\r");

    // attempt to write to past kernel code segment
    u32* pointer = kernel_end;
    pointer[0x1000] = 0;

    u32 i = 0;
    for(;;){
        ++i;
    }
/*
    struct idt_ptr ip;
    irq_get_curr_idt_ptr(&ip);
    serial_puts("\n\r");
    serial_puts("IDT pointer = ");
    serial_put_hex(ip.base);
    serial_puts("\n\r");
    serial_puts("IDT Size = ");
    serial_put_hex(ip.limit);
    serial_puts("\n\r");

    irq_init_idt();

    irq_get_curr_idt_ptr(&ip);
    serial_puts("\n\r");
    serial_puts("IDT pointer = ");
    serial_put_hex(ip.base);
    serial_puts("\n\r");
    serial_puts("IDT Size = ");
    serial_put_hex(ip.limit);
    serial_puts("\n\r");
    */
}