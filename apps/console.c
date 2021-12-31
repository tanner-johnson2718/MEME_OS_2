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

#define MAX_LINE_SIZE 256
u8 index = 0;
u8 line_buffer[MAX_LINE_SIZE];

void output_c(u8 c)
{
    vga_msg_t msg;
    msg.bg = bg;
    msg.fg = fg;
    msg.col = col;
    msg.row = row;
    msg.c = c;

    sched_app_publish_OUT_event((u8*) &msg, sizeof(vga_msg_t), SCHED_VGA_ID);
}

void output_s()
{
    
}

void clr_scr()
{

}

void update_screen_index(u8 c)
{
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

    if(c == '\n')
    {
        row++;
        col = 0;
        if(row == VGA_BUFFER_HEIGHT)
        {
            row = 0;
        }
    }
   
}

void insert_line_buffer(u8 c)
{
    if(index >= MAX_LINE_SIZE)
    {
        // BIG OOF, what to do?
    }

    line_buffer[index] = c;
    index++;
}

void process_line_buffer()
{

}

void input_handler()
{
    u8 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 num_read = 0;
    

    num_read =  sched_app_pop_IN_event(SCHED_PS2_ID, data, size);

    // Now parse each character from the input event
    u8 i = 0;
    for(i = 0; i < num_read; ++i)
    {

        output_c(data[i]);
        update_screen_index(data[i]);

        if(data[i] == '\n')
        {
            process_line_buffer();
        }
        else
        {
            insert_line_buffer(data[i]);
        }
    }
}

void console_init()
{
    sched_app_register_callback(input_handler);
}