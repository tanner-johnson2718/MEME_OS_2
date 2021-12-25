#include "sched/event.h"
#include "core/types.h"

u32 modID_next = 0;

u32 event_register_module()
{
    u32 ret = modID_next;
    modID_next++;
    return ret;
}