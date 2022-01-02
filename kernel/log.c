#include "types.h"
#include "log.h"
#include "serial.h"

// See log.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Private Register and Macros
///////////////////////////////////////////////////////////////////////////////

#define LOG_MSG_STR   "MSG"
#define LOG_WARN_STR  "WARNING"
#define LOG_ERROR_STR "ERROR"

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API Functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     log_msg

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u8* file  - The __LINE__ macro in c-string format
          2) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_msg(u8* file, u8* line, u8* c_str)
{

}



/******************************************************************************
NAME)     log_warning

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u8* file  - The __LINE__ macro in c-string format
          2) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_warning(u8* file, u8* line, u8* c_str)
{

}



/******************************************************************************
NAME)     log_error

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u8* file  - The __LINE__ macro in c-string format
          2) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_error(u8* file, u8* line, u8* msg)
{

}