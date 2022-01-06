#include "types.h"
#include "log.h"
#include "serial.h"
#include "timer.h"

// See log.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API Functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     log_msg

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u32 line  - The __LINE__ macro in integer format
          2) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0, always suceed

COMMENTS) NONE
******************************************************************************/
u8 log_msg(u8* file, u32 line, u8* c_str)
{
    serial_puts("[");
    u32 time = 0;
    timer_get_time_ms(&time);
    serial_putd(time);
    serial_puts("ms");
    serial_puts("]");

    // Put file
    serial_puts("[");
    serial_puts(file);
    serial_puts(":");
    serial_putd(line);
    serial_puts("]");

    // put message
    serial_puts(" ");
    serial_puts(c_str);
    serial_puts("\n\r");

    return 0;
}