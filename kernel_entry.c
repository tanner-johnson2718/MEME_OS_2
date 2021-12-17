// Entry point for the kernel
#include "uart_8250.h"
#include "gdt.h"
#include "types.h"
#include "irq.h"
#include "ps2.h"

extern u32 kernel_end;

void kernel_main(void) 
{
    serial_init();
    gdt_install();
    irq_init();
    ps2_init();

    // asm volatile("int $0x20");

    while(1)
    {
        // IDLE
    }
}