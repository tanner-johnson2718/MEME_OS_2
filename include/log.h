#ifndef MEME_LOG
#define MEME_LOG

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

// Kernel and user space logging library. Will provide functions for logging
// and error output to the serial console. Will allow for passing of the file
// and line macro. 


///////////////////////////////////////////////////////////////////////////////
// Kernel Public API functions
/////////////////////////////////////////////////////////////////////////////// 

/******************************************************************************
NAME)     log_msg

INPUTS)
          0) u8* file  - The __FILE__ macro in c-string format
          1) u32 file  - The __LINE__ macro in integer format
          2) u8* c_str - A pointer to a c_str message

OUTPUTS)  NONE

RETURNS)  0, always succedds

COMMENTS) NONE
******************************************************************************/
u8 log_msg(u8* file, u32 line, u8* c_str);

#endif