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
          2) u8* c_str - A pointer to a c_str message

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



/******************************************************************************
NAME)     log_val

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u32 file  - The __LINE__ macro in integer format
          2) u8* c_str - A pointer to a c_str message
          3) u32 val   - The value to be logged

OUTPUTS)  NONE

RETURNS)  0, always succedds

COMMENTS) NONE
******************************************************************************/
u8 log_val(u8* file, u32 line, u8* c_str, u32 val)
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

    // put value
    serial_puts(" : ");
    serial_put_hex(val);
    serial_puts("\n\r");
    
    return 0;
}


/******************************************************************************
NAME)     log_data8

INPUTS)
          0) u8* c_str - A pointer to a data to dump
          1) u8 size   - Number of bytes in data to dump

OUTPUTS)  NONE

RETURNS)  0, always suceed

COMMENTS) NONE
******************************************************************************/
u8 log_data8(u8* data, u8 size)
{
    u8 i = 0;
    for(i = 0; i < size; ++i)
    {
        if(i % 16 == 0){ serial_puts("\n\r"); }

        serial_put_hex( ((u8) *(data+i)) );
        serial_puts(" ");
    }

    serial_puts("\n\r\n\r");

    return 0;
}