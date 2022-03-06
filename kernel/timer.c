#include "timer.h"
#include "types.h"
#include "io_port.h"
#include "irq.h"
#include "log.h"

///////////////////////////////////////////////////////////////////////////////
// Private MACROs, registers, and functions
///////////////////////////////////////////////////////////////////////////////

u32 time_ms = 1;

#define TIMER_MAX_CLOCK 1193182
#define TIMER_PORT0 0x40
#define TIMER_PORT1 0x41
#define TIMER_PORT2 0x42
#define TIMER_PORT3 0x43

#define CHAN0_SQR_WAVE 0x36

void timer_interrupt_handler()
{
    time_ms++;
}

void set_timer_phase(u32 hz)
{
    int divisor = TIMER_MAX_CLOCK / hz;
    outb(TIMER_PORT3, CHAN0_SQR_WAVE);       /* Set our command byte 0x36 */
    outb(TIMER_PORT0, divisor & 0xFF);       /* Set low byte of divisor */
    outb(TIMER_PORT0, divisor >> 8);         /* Set high byte of divisor */
}

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API
///////////////////////////////////////////////////////////////////////////////


/******************************************************************************
NAME)     timer_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 timer_init()
{
    set_timer_phase(1000);
    
    if( irq_register_PIC_handler(timer_interrupt_handler, IRQ_PIC_TIMER) )
    {
        log_msg(__FILE__, __LINE__, "Failed to register timer IRQ handler");
        return TIMER_FAIL_REG_IRQ;
    }

    log_msg(__FILE__, __LINE__, "System heartbeat");

    return 0;
}



/******************************************************************************
NAME)     timer_get_time_ms

INPUTS)   
          0) time - pointer to 32bit value to hold time

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 timer_get_time_ms(u32* time)
{
    *time = time_ms;
    return 0;
}


