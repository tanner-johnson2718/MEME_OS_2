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
    irq_init_idt();
    irq_raise255();
    while(1){};
}