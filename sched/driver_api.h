#ifndef MEME_SCHED_DRIVER_API
#define MEME_SCHED_DRIVER_API

#include "core/types.h"

typedef struct
{
    u8* data;
    u32 size;
} event_t;

void sched_publish_event(event_t event);

#endif