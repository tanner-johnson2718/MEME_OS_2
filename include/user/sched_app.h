#ifndef MEME_SCHED_APP_API
#define MEME_SCHED_APP_API

#include "types.h"

u8 sched_app_publish_OUT_event(u8* data, u32 size, u32 driverID);
u8 sched_app_pop_IN_event(u32 driverID, u8* buffer, u32 size);
u8 sched_app_register_callback(void (*handler)(void), u32 driverID);

#endif