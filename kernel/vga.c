#include "types.h"
#include "vga.h"
#include "event.h"
#include "sched_driver.h"

u16* get_loc_text_buffer(u8 i, u8 j)
{
    // Priavte function assume indicies are good

    u16 index = (j * VGA_BUFFER_WIDTH) + i;
    return ((u16*)(VGA_TEXT_BUFFER)) + index;
}

u16 build_buffer_entry(u8 bg, u8 fg, u8 c)
{
    // Priavte function assume input values are good

    u16 ret = c;
    ret |= (fg << VGA_FG_SHIFT);
    ret |= (bg << VGA_BG_SHIFT);
    return ret;
}

void put_entry_text_buff(u8 bg, u8 fg, u8 c, u8 i, u8 j)
{
    // Priavte function assume input values are good

    u16 entry = build_buffer_entry(bg, fg, c);
    u16* loc_16 = get_loc_text_buffer(i,j);
    if(loc_16 > 0)
    {
        *loc_16 = entry;
    }
    
}

// 0x1 - Bad buffer 
u8 check_input(u8 x, u8 y, u8 fg, u8 bg)
{
    if(x >= VGA_BUFFER_WIDTH || y >= VGA_BUFFER_HEIGHT)
    {
        return VGA_OUT_BOUNDS_INDICES;
    }

    if(fg > VGA_WHITE | bg > VGA_WHITE)
    {
        return VGA_INVALID_COLOR;
    }

    return 0x0;
}

// Public

void vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg)
{
    if(check_input(x,y,bg,fg))
    {
        return;
    }

    put_entry_text_buff(bg, fg, c, x, y);
}

u8 vga_textmode_getc(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return (*entry) & 0xff;
}

u8 vga_textmode_get_fg(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return ((*entry) >> VGA_FG_SHIFT) & 0xf;
}

u8 vga_textmode_get_bg(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return ((*entry) >> VGA_BG_SHIFT) & 0xf;
}

// The simple vga out put call back will simply take in characters from the
// output buffer and write them sequentially to screen, rolling back on rows
// and cols as neded. FG and BG are fixed.
u32 row = 0;
u32 col = 0;
void simple_vga_output_callback()
{
    // Declare small stack buffers for copying message
    u32 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 i = 0;
    u8 num_read = 0;

    // Pop messages off the buffer until no more remain,
    // for each message loop over the bytes and output
    while( (num_read = sched_driver_pop_OUT_event(SCHED_VGA_ID, data, size)) )
    {
        for(i = 0; i < num_read; ++i)
        {
            vga_textmode_putc(col, row, data[i], VGA_WHITE, VGA_BLACK);
            col++;

            if(col >= VGA_BUFFER_WIDTH)
            {
                col = 0;
                row++;
            }

            if(row >= VGA_BUFFER_HEIGHT)
            {
                row = 0;
            }
        }
    }
}

void vga_init()
{
    // Maybe verify that the VGA driver is set to text mode?
    // Maybe allow input to allow different color modes? actually set pixels

    // register output handler
    sched_driver_register_callback(simple_vga_output_callback);
}