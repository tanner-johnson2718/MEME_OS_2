#ifndef MEME_LOG
#define MEME_LOG

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

// Kernel and user space logging library. Will provide functions for logging
// and error output to the serial console. There will be 3 debug levels, log
// is just a message send to the user, warning is a warning that something is
// potentailly not right, and error is a serious error report. Each level will
// have its own logging function. Moreover the log message will contain the
// the level it was logged. Finally macros will be provided to put location of
// log message within code in the log message.

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
          0) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'msg' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_msg(u8* c_str);



/******************************************************************************
NAME)     log_warning

INPUTS)
          0) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'warning' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_warning(u8* c_str);



/******************************************************************************
NAME)     log_error

INPUTS)
          0) u8* c_str - A pointer to a c_str i.e. null terminated string to
                         output at a 'error' level

OUTPUTS)  NONE

RETURNS)  0 on sucess and valid error code on error

COMMENTS) NONE
******************************************************************************/
u8 log_error(u8* c_str);

#endif