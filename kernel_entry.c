// Entry point for the kernel
#include "core/gdt.h"
#include "core/types.h"
#include "core/irq.h"
#include "drivers/serial.h"
#include "drivers/ps2.h"

void kernel_main(void) 
{
    gdt_install();
    irq_init();
    serial_init();
    ps2_init();

    // asm volatile("int $0x20");

    while(1)
    {
        // IDLE
    }
}