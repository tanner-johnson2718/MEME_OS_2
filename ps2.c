#include "ps2.h"
#include "types.h"
#include "io_port.h"
#include "uart_8250.h"
#include "irq.h"

void ps2_keyboard_irq(void)
{
    u8 in = inb(PS2_DATA_PORT);
    serial_putc(in);
}

void ps2_init(void)
{
    irq_register_PIC_handler(ps2_keyboard_irq, PIC_KEY);
}