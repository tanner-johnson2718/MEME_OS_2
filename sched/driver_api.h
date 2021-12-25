#ifndef MEME_SCHED_DRIVER_API
#define MEME_SCHED_DRIVER_API

#include "sched/event.h"
#include "core/types.h"

void sched_publish_IN_event(u32* data, u32 size, u32 modID);
void sched_check_OUT_event(u32 modID);
void sched_pop_OUT_event(u32 modID);

#endif