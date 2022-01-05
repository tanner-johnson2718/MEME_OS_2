#include "types.h"
#include "log.h"
#include "serial.h"
#include "timer.h"

// See log.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Private Register and Macros
///////////////////////////////////////////////////////////////////////////////

#define LOG_MAX_MSG_LEN 255        // 256-1 to fit '\0'

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

// assumes enough room, access a max of 10 bytes
u8 dtos(u32 d, u8* num)
{
    u32 m = 10;
    num[0] = 0;
    u32 i;
    for(i = 0; (i<m); ++i)
    {
        num[i] = d % 10;
        d = d / 10;

        if(d == 0)
        {
            break;
        }
    }

    return i;
}

u8 place_in_buffer(u8 i, u8* buffer, u8* str)
{
    u8 j = 0;
    while( (i < LOG_MAX_MSG_LEN) && (str[j] != '\0') )
    {
        buffer[i] = str[j];
        ++i;
        ++j;
    }
    
    return i;
}

// Expects buffer to be of len 256
u8 build_message(u8* buffer, u8* file, u32 line, u8* msg)
{
    u32 time = 0;
    timer_get_time_ms(&time);

    u8 i = 0;

    // Put [time]
    buffer[i] = '[';
    ++i;
    i+= dtos(time, buffer + i);
    buffer[i] = ']';
    ++i;

    // Put [file], need to start checking i during file name insert
    buffer[i] = '[';
    ++i;

    i = place_in_buffer(i,buffer,file);
    if(i == LOG_MAX_MSG_LEN)
    {
        return i;
    }

    buffer[i] = ']';
    ++i;
    if(i == LOG_MAX_MSG_LEN)
    {
        return i;
    }

    // Put [line]
    buffer[i] = '[';
    ++i;
    if(i == LOG_MAX_MSG_LEN)
    {
        return i;
    }

    u8 line_str[11];
    u8 line_index = dtos(line, line_str);
    line_str[line_index] = '\0';
    i = place_in_buffer(i,buffer,line_str);
    if(i == LOG_MAX_MSG_LEN)
    {
        return i;
    }

    buffer[i] = ']';
    ++i;
    if(i == LOG_MAX_MSG_LEN)
    {
        return i;
    }
    
    // Put " <msg>"
    
}

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

RETURNS)  0 on sucess and valid error code on error

COMMENTS) If the resultant log is >255 bytes, message will be truncated
******************************************************************************/
u8 log_msg(u8* file, u32 line, u8* c_str)
{

}