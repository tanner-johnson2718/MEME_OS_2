#include "types.h"
#include "vga.h"
#include "console.h"
#include "log.h"
#include "ps2.h"

// See console.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Private Defines and state
///////////////////////////////////////////////////////////////////////////////

u8 bg = VGA_BLACK;
u8 fg = VGA_WHITE;
u8 row = 0;
u8 col = 0;

#define MAX_LINE_SIZE 256
u8 line_index = 0;
u8 line_buffer[MAX_LINE_SIZE];

char** cmd_list = {
    "clear",
    "read",
    "write",
    "examine",
    "call"
};

///////////////////////////////////////////////////////////////////////////////
// Private Command Functions
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

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
        col = 0;
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

    vga_textmode_update_cursor(col,row);
}

void process_line()
{

}

void kb_hanle(u8 in)
{
    if(in != '\n')
    {
        vga_textmode_putc(col, row, in, VGA_WHITE, VGA_BLACK);

        line_buffer[line_index] = in;
        line_index++;
        
        if(line_index == (MAX_LINE_SIZE-1))
        {
            log_msg(__FILE__,__LINE__,"Maximum line size reached!");
            line_index = 0;
        }
    }
    else
    {
        process_line();
    }
    
    update_screen_index(in);
}

///////////////////////////////////////////////////////////////////////////////
// Public
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     console_entry

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 always succeeds

COMMENTS) NONE
******************************************************************************/
u8 console_entry()
{
    // Some test code
    vga_textmode_clear_screen(fg, bg);
    vga_textmode_update_cursor(col,row);
    ps2_register_hanlder(kb_hanle);

    while(1)
    {
        // IDLE
    }
}