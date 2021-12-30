#include "types.h"
#include "sched_app.h"
#include "event.h"

void serial_loopback()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;

    num_read =  sched_app_pop_IN_event(SCHED_SERIAL_ID, data, size);
    sched_app_publish_OUT_event(data, num_read, SCHED_SERIAL_ID);
}

void ps2_to_vga()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;

    num_read =  sched_app_pop_IN_event(SCHED_PS2_ID, data, size);
    sched_app_publish_OUT_event(data, num_read, SCHED_VGA_ID);
}

void console_init()
{
    sched_app_register_callback(serial_loopback);
}