// Entry point for the kernel
#include "uart_8250.h"

extern int kernel_start;
extern int kernel_end;

void kernel_main(void) 
{

    serial_puts("The Divisor Latch is: ");
    serial_putd(serial_get_buad());
    serial_puts("\n");

    int i = 0;
    for(;;){
        ++i;
    }
}