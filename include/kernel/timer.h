#ifndef MEME_TIMER
#define MEME_TIMER

#include "types.h"

#define TIMER_MAX_CLOCK 1193180
#define TIMER_PORT0 0x40
#define TIMER_PORT1 0x41
#define TIMER_PORT2 0x42
#define TIMER_PORT3 0x43

void timer_init();
u32 timer_get_time_ms();

#endif