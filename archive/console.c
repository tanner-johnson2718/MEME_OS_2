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
u8 line_index = 0;
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

// Output a string w/ out a new line
void output_s(u8* c_str)
{
    u8 i = 0;
    u8 j = 0;
    vga_msg_t msgs[VGA_MSG_PER_EVENT] = {0};

    while(c_str[i] != '\0')
    {

        msgs[i].bg = bg;
        msgs[i].fg = fg;
        msgs[i].col = col;
        msgs[i].row = row;
        msgs[i].c = c_str[j];

        ++i;
        ++j;
        if(i == VGA_MSG_PER_EVENT)
        {   
            // Messages full send
            sched_app_publish_OUT_event((u8*)msgs, i*sizeof(vga_msg_t), SCHED_VGA_ID);
            update_screen_index(c_str[j]);
            i = 0;
        }
    }

    // clear what is left
    sched_app_publish_OUT_event((u8*)msgs, i*sizeof(vga_msg_t), SCHED_VGA_ID);
    update_screen_index(c_str[j]);
}

void clr_scr()
{
    output_c(VGA_CLR_SCREEN_CHAR);
    row = 0;
    col = 0;
}

void insert_line_buffer(u8 c)
{
    if(line_index >= MAX_LINE_SIZE)
    {
        // BIG OOF, what to do?
        return;
    }

    line_buffer[line_index] = c;
    line_index++;
}

void process_line_buffer()
{
    line_buffer[line_index] = (u8)0;
    line_index = 0;
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
        

        if(data[i] == '\n')
        {
            process_line_buffer();
        }
        else
        {
            insert_line_buffer(data[i]);
            output_c(data[i]);
        }

        update_screen_index(data[i]);
    }
}

void console_init()
{
    sched_app_register_callback(input_handler);
    clr_scr();
}