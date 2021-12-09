// Entry point for the kernel
#include "uart_8250.h"
#include "gdt.h"
#include "types.h"

void kernel_main(void) 
{

    serial_init();
    gdt_install();

    u32 i = 0;
    for(;;){
        ++i;
    }
}