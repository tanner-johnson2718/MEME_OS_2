#ifndef MEME_SCHED_DRIVER_API
#define MEME_SCHED_DRIVER_API

#include "sched/event.h"
#include "core/types.h"

void sched_driver_publish_IN_event(u32* data, u32 size, u32 driverID);
void sched_driver_check_OUT_event(u32 driverID);
void sched_driver_pop_OUT_event(u32 driverID);

#endif