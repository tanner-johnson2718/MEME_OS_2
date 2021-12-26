#include "sched/app_api.h"
#include "sched/driver_api.h"
#include "sched/event.h"
#include "drivers/timer.h"

#define BUFFER_SIZE 256*1024   // in events which are 256b

event_t in_buffer[BUFFER_SIZE] = {0};    // 64MB
event_t out_buffer[BUFFER_SIZE] = {0};   // 64MB

event_t* oldest_in = in_buffer;
event_t* next_in = in_buffer;
event_t* oldest_out = out_buffer;
event_t* next_out = out_buffer;

//-----------------------------------------------------------------------------
// helper
//-----------------------------------------------------------------------------

void inc_pointer(event_t* p, event_t* wrap)
{
    ++p;
    if((event_t*)p == (event_t*)wrap + BUFFER_SIZE)
    {
        p = wrap;
    }
}

// When elements popped or published need to update index pointers, sub routine
// to do that
void clean_buffers()
{
    // Check if next_in needs updating
    if(next_in->time_added != 0)
    {
        inc_pointer(next_in, in_buffer);
    }

    // Check if next_out needs updating
    if(next_out->time_added != 0)
    {
        inc_pointer(next_out, out_buffer);
    }

    // check if oldest_in needs updating
    if(oldest_in->time_popped != 0)
    {
        // scan till you hit un-popped element
        while(oldest_in != next_in)
        {
            if(oldest_in->time_popped == 0)
            {
                break;    // done, found oldest un-popped
            }

            inc_pointer(oldest_in, in_buffer);
        }
    }

    // check if oldest_out needs updating
    if(oldest_out->time_popped != 0)
    {
        // scan till you hit un-popped element
        while(oldest_out != next_out)
        {
            if(oldest_out->time_popped == 0)
            {
                break;    // done, found oldest un-popped
            }

            inc_pointer(oldest_out, out_buffer);
        }
    }
}

void generic_pub(u8* data, u32 size, u32 driverID, event_t* next)
{
    // Copy data
    u32 i = 0;
    u8* target = (u8*) &(next->data);
    for(; i < size; ++i)
    {
        target[i] = data[i];
    }

    // Copy over meta data
    next->size = size;
    next->driverID = driverID;
    next->time_added = timer_get_time_ms();
    next->time_popped = 0;

    // Update pointers
    clean_buffers();
}

u8 generic_pop(u32 driverID, u8* buffer, u32 size, event_t* next, event_t* oldest, event_t* event_buffer)
{
    u8 ret = 0;

    // Scan the in buffer for events matching the driver ID
    event_t* temp = oldest;
    while((event_t*)temp != (event_t*)next)
    {
        if(temp->driverID == driverID)
        {
            ret = 1;
            break;
        }

        inc_pointer(temp, event_buffer);
    }

    if(!ret)
    {
        return ret;
    }

    // found element to pop, copy contents to app buffer and set pop time
    u32 i = 0;
    u8* target = (u8*) &(temp->data);
    for(; i < size; ++i)
    { 
        buffer[i] = target[i];
    }
    temp->time_popped = timer_get_time_ms();

    // clean up buffer to remove popped elements and update pointers
    clean_buffers();

    return ret;
}

//-----------------------------------------------------------------------------
// Driver
//-----------------------------------------------------------------------------

void sched_driver_publish_IN_event(u8* data, u32 size, u32 driverID)
{   
    generic_pub(data, size, driverID, next_in);
}

void sched_driver_pop_OUT_event(u32 driverID, u8* buffer, u32 size)
{
    return generic_pop(driverID, size, buffer, next_in, oldest_in, in_buffer);
}

//-----------------------------------------------------------------------------
// App
//-----------------------------------------------------------------------------

void sched_app_publish_OUT_event(u8* data, u32 size, u32 driverID)
{
    generic_pub(data, size, driverID, next_out);
}

u8 sched_app_pop_IN_event(u32 driverID, u8* buffer, u32 size)
{
    return generic_pop(driverID, size, buffer, next_in, oldest_in, in_buffer);
}