#ifndef MEME_SCHED_APP_API
#define MEME_SCHED_APP_API

#include "sched/event.h"
#include "core/types.h"

void sched_app_publish_OUT_event(u32* data, u32 size, u32 driverID);
void sched_app_check_IN_event(u32 driverID);
void sched_app_pop_IN_event(u32 driverID);

#endif