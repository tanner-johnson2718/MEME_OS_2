#include "app_api.h"
#include "driver_api.h"
#include "event.h"
#include "sched.h"
#include "timer.h"

// Test
#include "serial.h"

// Just needs to big enough to clear on an interrupt event. Ideally
// should fit on a L3 cache line to avoid perf issues of O(n) scans
// of the buffers.
#define BUFFER_SIZE 64  

// Circular input / output buffers. The time stamps of the event structure form
// a state machine indicating the state of any given event entry:
// | time_added | time_popped | state   |
// |------------|-------------|---------|
// |    >=0     |     =0      | un-proc |
// |    >0      |     >0      | proc    |
event_t in_buffer[BUFFER_SIZE] = {0};    // 64MB
event_t out_buffer[BUFFER_SIZE] = {0};   // 64MB

// Pointers to index the oldest and next place to put an entry on the buffers
// Again where these pointer point form a state machine (kinda):
// Buffer is empty iff next == oldest after a pop event or on boot
// Buffer is full  iff next == oldest after a pub event
// Buffer will never be left in full state.
event_t* oldest_in = in_buffer;
event_t* next_in = in_buffer;
event_t* oldest_out = out_buffer;
event_t* next_out = out_buffer;

//-----------------------------------------------------------------------------
// helper
//-----------------------------------------------------------------------------

u8 event_is_un_processed(event_t* e)
{
    return ((e->time_added >= 0) && (e->time_popped == 0));
}

u8 event_is_processed(event_t* e)
{
    return ((e->time_added > 0) && (e->time_popped > 0));
}

u8 buffer_is_empty(event_t* next, event_t* oldest)
{
    return (next == oldest);
}

void inc_pointer(event_t** p, event_t* wrap)
{
    ++(*p);
    if((event_t*)(*p) == (event_t*)wrap + BUFFER_SIZE)
    {
        (*p) = wrap;
    }
}

u8 generic_pub(u8* data, u32 size, u32 driverID, event_t** next, event_t** oldest)
{
    if(size > EVENT_DATA_SIZE)
    {
        return 0;
    }

    // Copy data
    u32 i = 0;
    u8* target = (u8*) &((*next)->data);
    for(; i < size; ++i)
    {
        target[i] = data[i];
    }

    // Copy over meta data
    (*next)->size = size;
    (*next)->driverID = driverID;
    (*next)->time_added = timer_get_time_ms();
    (*next)->time_popped = 0;

    // On a publish event, the next pointer needs to moved to the right by one,
    // wrapped if nes. and if buffer full, then need to moves oldest to right
    // by one as well
    inc_pointer(next, in_buffer);
    if(*next == *oldest)
    {
        inc_pointer(oldest, in_buffer);
    }

    return 1;
}

u32 generic_pop(u32 driverID, u8* buffer, u32 size, event_t** next, event_t** oldest, event_t* event_buffer)
{
    u8 ret = 0;

    // Scan the in buffer for events matching the driver ID
    event_t* temp = *oldest;
    while((event_t*)temp != (event_t*)(*next))
    {
        if(temp->driverID == driverID)
        {
            ret = 1;
            break;
        }

        inc_pointer(&temp, event_buffer);
    }

    if(!ret)
    {
        return ret;
    }

    // found element to pop, copy contents to app buffer and set pop time
    u32 i = 0;
    u8* target = (u8*) &(temp->data);
    for(; i < size && i < temp->size; ++i)
    { 
        buffer[i] = target[i];
    }
    temp->time_popped = timer_get_time_ms();

    // On a pop event one could pop an entry be anywhere inbetween oldest and
    // next. So we want to make sure we move oldest an appropoate amount based
    // on if there is a linear sequence of processed events from oldest to the
    // the next oldeset un-processed event.
    while(((!(buffer_is_empty(*next, *oldest))) && event_is_processed(*oldest)))
    {
        // buffer is not empty and oldest is processed, so inc oldest
        inc_pointer(oldest,event_buffer);
    }

    // only exits loop if buffer is empty or oldest is pointing to an un-proced
    // event.

    // return num bytes read
    return i;   
}

//-----------------------------------------------------------------------------
// Driver
//-----------------------------------------------------------------------------

u8 sched_driver_publish_IN_event(u8* data, u32 size, u32 driverID)
{   
    return generic_pub(data, size, driverID, &next_in, &oldest_in);
}

u32 sched_driver_pop_OUT_event(u32 driverID, u8* buffer, u32 size)
{
    return generic_pop(driverID, buffer, size, &next_out, &oldest_out, out_buffer);
}

//-----------------------------------------------------------------------------
// App
//-----------------------------------------------------------------------------

u8 sched_app_publish_OUT_event(u8* data, u32 size, u32 driverID)
{
    return generic_pub(data, size, driverID, &next_out, &oldest_in);
}

u32 sched_app_pop_IN_event(u32 driverID, u8* buffer, u32 size)
{
    return generic_pop(driverID, buffer, size, &next_in, &oldest_in, in_buffer);
}

//-----------------------------------------------------------------------------
// API Test
//-----------------------------------------------------------------------------

void dump_buffer(event_t* buffer, event_t* next, event_t* oldest)
{
    serial_puts("Base Addr   = ");
    serial_put_hex((u32) buffer);
    serial_puts("\n\r");
    serial_puts("Oldest Addr = ");
    serial_put_hex((u32) oldest);
    serial_puts("\n\r");
    serial_puts("Next Addr   = ");
    serial_put_hex((u32) next);
    serial_puts("\n\r");
    serial_puts("\n\r");
    
    event_t* temp = oldest;
    while(temp != next)
    {
        serial_puts("   Addr   = ");
        serial_put_hex((u32) temp);
        serial_puts("\n\r");

        serial_puts("   Data   = ");
        u32 i = 0;
        u8* target = (u8*) &(temp->data);
        for(; i < temp->size; ++i)
        {
            serial_put_hex((u32) target[i]);
            serial_puts(" ");
        }
        serial_puts("\n\r");

        serial_puts("   Size  = ");
        serial_put_hex((u32) temp->size);
        serial_puts("\n\r");

        serial_puts("   Time A = ");
        serial_put_hex((u32) temp->time_added);
        serial_puts("\n\r");

        serial_puts("   Time P = ");
        serial_put_hex((u32) temp->time_popped);
        serial_puts("\n\r");

        serial_puts("   Dri ID = ");
        serial_put_hex((u32) temp->driverID);
        serial_puts("\n\r");
        serial_puts("\n\r");

        inc_pointer(&temp, in_buffer);
    }
}

void sched_dump_event_buffers()
{
    serial_puts("In Buffer)\n\r");
    dump_buffer(in_buffer, next_in, oldest_in);
    serial_puts("\n\r");

    serial_puts("Out Buffer)\n\r");
    dump_buffer(out_buffer, next_out, oldest_out);
    serial_puts("\n\r");
}

//-----------------------------------------------------------------------------
// Scheduler execution thread
//-----------------------------------------------------------------------------

u32 interrupt_depth = 0;

void sched_thread()
{
    // am i entering an already interrupted context?
    interrupt_depth++;
    if(interrupt_depth > 1)
    {
        // I am interrupting an already running scheduler
    }

    // wake up applications

    // apps done, double check that I haven't interrupted

    // ensure input buffer cleared

    // wake up drivers

    // drivers done

    // ensure output buffer cleared

    // done
    interrupt_depth--;
    return;
}