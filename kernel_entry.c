// Entry point for the kernel
#include "uart_8250.h"
#include "gdt.h"

extern int kernel_start;
extern int kernel_end;

void kernel_main(void) 
{

    serial_init();
    serial_puts("\n\r");
    serial_put_hex(0);
    
    // Do not know if I actually need to set up GDT, but code is here?
    // gdt_install();

    int i = 0;
    for(;;){
        ++i;
    }
}