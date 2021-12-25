#include "drivers/timer.h"
#include "core/types.h"
#include "core/io_port.h"
#include "core/irq.h"

u32 time_ms = 0;

void set_timer_phase(u32 hz)
{
    int divisor = TIMER_MAX_CLOCK / hz;
    outb(TIMER_PORT3, 0x36);                 /* Set our command byte 0x36 */
    outb(TIMER_PORT0, divisor & 0xFF);       /* Set low byte of divisor */
    outb(TIMER_PORT0, divisor >> 8);         /* Set high byte of divisor */
}

void timer_interrupt_handler()
{
    time_ms++;
}

u32 timer_get_time_ms()
{
    return time_ms;
}

void timer_init()
{
    set_timer_phase(1000);
    irq_register_PIC_handler(timer_interrupt_handler, IRQ_PIC_TIMER);
}