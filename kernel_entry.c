// Entry point for the kernel
#include "uart_8250.h"
#include "gdt.h"
#include "types.h"
#include "irq.h"

extern u32 kernel_end;

void kernel_main(void) 
{

    serial_init();
    gdt_install();

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

    // try to trigger a div by 0
    irq_raise33();

   u32 i = 0;
   for(;;)
   {

   }
    
}