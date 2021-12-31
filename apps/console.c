#include "types.h"
#include "sched_app.h"
#include "vga_msg.h"

//-----------------------------------------------------------------------------
// Test
//-----------------------------------------------------------------------------
void serial_loopback()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;

    num_read =  sched_app_pop_IN_event(SCHED_SERIAL_ID, data, size);
    sched_app_publish_OUT_event(data, num_read, SCHED_SERIAL_ID);
}

void ps2_to_vga_simple()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;

    num_read =  sched_app_pop_IN_event(SCHED_PS2_ID, data, size);
    sched_app_publish_OUT_event(data, num_read, SCHED_VGA_ID);
}

//-----------------------------------------------------------------------------
// Full implementation
//-----------------------------------------------------------------------------

u8 bg = VGA_BLACK;
u8 fg = VGA_WHITE;
u8 row = 0;
u8 col = 0;

void ps2_to_vga_full()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;
    vga_msg_t msg;

    num_read =  sched_app_pop_IN_event(SCHED_PS2_ID, data, size);

    // Now parse each character from the input event
    u8 i = 0;
    for(i = 0; i < num_read; ++i)
    {
        if(data[i] == '\n')
        {
            row++;
            if(row == VGA_BUFFER_HEIGHT)
            {
                row = 0;
                col = 0;
            }

            continue;
        }

        msg.bg = bg;
        msg.fg = fg;
        msg.col = col;
        msg.row = row;
        msg.c = data[i];

        sched_app_publish_OUT_event((u8*) &msg, sizeof(vga_msg_t), SCHED_VGA_ID);
        
        col++;
        if(col == VGA_BUFFER_WIDTH)
        {
            row++;
            col = 0;
        }

        if(row == VGA_BUFFER_HEIGHT)
        {
            row = 0;
        }
    }
}

void console_init()
{
    sched_app_register_callback(ps2_to_vga_full);
}