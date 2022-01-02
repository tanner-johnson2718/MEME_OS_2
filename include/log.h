#ifndef MEME_LOG
#define MEME_LOG

// Kernel and user space logging library. Will provide functions for logging
// and error output to the serial console. There will be 3 debug levels, log
// is just a message send to the user, warning is a warning that something is
// potentailly not right, and error is a serious error report.

void log_msg(u8* c_str);
void log_warning(u8* c_str); 
void log_error(u8* c_str);

#endif