#ifndef MEME_SCHED_DRIVER_API
#define MEME_SCHED_DRIVER_API

#include "sched/event.h"
#include "core/types.h"

u8 sched_driver_publish_IN_event(u8* data, u32 size, u32 driverID);
u32 sched_driver_pop_OUT_event(u32 driverID, u8* buffer, u32 size);

// Test
void sched_dump_event_buffers();

#endif