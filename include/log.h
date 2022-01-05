#ifndef MEME_LOG
#define MEME_LOG

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

// Kernel and user space logging library. Will provide functions for logging
// and error output to the serial console. Will allow for passing of the file
// and line macro. 

///////////////////////////////////////////////////////////////////////////////
// Kernel Public Macros
///////////////////////////////////////////////////////////////////////////////
#define LOG_ERROR_1 1   // unsued for now

///////////////////////////////////////////////////////////////////////////////
// Kernel Public API functions
/////////////////////////////////////////////////////////////////////////////// 

/******************************************************************************
NAME)     log_msg

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u32 file  - The __LINE__ macro in integer format
          2) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) If the resultant log is >255 bytes, message will be truncated
******************************************************************************/
u8 log_msg(u8* file, u32 line, u8* c_str);

#endif