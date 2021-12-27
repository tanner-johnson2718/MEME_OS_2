#ifndef MEME_SCHED_EVENT
#define MEME_SCHED_EVENT

#include "core/types.h"

// Set an arbitrary data size limit for an event message
// set s.t. the overal event_t size is 256 bytes
#define EVENT_DATA_SIZE 240

// Driver ID's so that we can target a specific driver
#define SCHED_PS2_ID 0
#define SCHED_SERIAL_ID 1
#define SCHED_TIMER_ID 2
#define SCHED_VGA_ID 3

typedef struct
{
    u8 data[EVENT_DATA_SIZE];
    u32 size;
    u32 time_added;
    u32 time_popped;
    u32 driverID;      // driver associated with this event
} event_t;

#endif