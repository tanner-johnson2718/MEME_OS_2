#ifndef MEME_EVENT
#define MEME_EVENT

#include "core/types.h"

// Set an arbitrary data size limit for an event message
// set s.t. the overal event_t size is 256 bytes
#define EVENT_DATA_SIZE 236

typedef struct
{
    u8 data[EVENT_DATA_SIZE];
    u32 size;
    u32 time_added;
    u32 time_popped;
    u32 modID_pub;      // Whose pushing this event
    u32 modID_sub;      // Whose popping and handling the event
} event_t;

// returns a module ID
u32 event_register_module();

#endif