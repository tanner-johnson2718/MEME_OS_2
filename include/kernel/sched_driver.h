#ifndef MEME_SCHED_DRIVER_API
#define MEME_SCHED_DRIVER_API

#include "types.h"

u8 sched_driver_publish_IN_event(u8* data, u8 size, u8 driverID);
u8 sched_driver_pop_OUT_event(u8 driverID, u8* buffer, u8 size);
u8 sched_driver_register_callback(void (*handler)());

// Test
// void sched_dump_event_buffers();

#endif