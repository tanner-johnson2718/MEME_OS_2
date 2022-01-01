#include "types.h"
#include "vga.h"
#include "sched_kernel.h"
#include "serial.h"

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

void vga_clear_screen(u8 bg, u8 fg)
{
    u32 i, j = 0;
    for(; i < VGA_BUFFER_HEIGHT; ++i)
    {
        for(; j < VGA_BUFFER_WIDTH; ++j)
        {
            vga_textmode_putc(j, i, 'a', fg, bg);
        }
    }
}

void simple_vga_output_callback()
{
    // Declare small stack buffers for copying message
    u32 size = EVENT_DATA_SIZE;
    u8 data[size];
    u8 i = 0;
    u8 num_read = 0;
    vga_msg_t *temp;

    // Pop messages off the buffer until no more remain,
    // for each message loop over the bytes and output
    while( (num_read = sched_driver_pop_OUT_event(SCHED_VGA_ID, data, size)) )
    {
        if( (num_read % sizeof(vga_msg_t)) != 0)
        {
            serial_puts("VGA ERROR read message of incorrect size\n\r");
            continue;
        }

        u8 n_msg = num_read / sizeof(vga_msg_t);
        temp = (vga_msg_t*) data;
        for(i = 0; i < n_msg; ++i)
        {
            if(temp->c == VGA_CLR_SCREEN_CHAR)
            {
                vga_clear_screen(temp->bg, temp->fg);
            }
            else
            {
                vga_textmode_putc(temp->col, temp->row, temp->c, temp->fg, temp->bg);
            }
            
            temp++;
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