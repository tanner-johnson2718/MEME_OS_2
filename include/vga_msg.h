#ifndef MEME_VGA_MSG
#define MEME_VGA_MSG

#include "types.h"
#include "sched_app.h"

// 16 Bit color values
#define VGA_BLACK    0x0
#define VGA_BLUE     0x1
#define VGA_GREEN    0x2
#define VGA_CYAN     0x3
#define VGA_RED      0x4
#define VGA_MAGENTA  0x5
#define VGA_BROWN    0x6
#define VGA_LGRAY    0x7
#define VGA_DGRAY    0x8
#define VGA_LBLUE    0x9
#define VGA_LGREEN   0xA
#define VGA_LCYAN    0xB
#define VGA_LRED     0xC
#define VGA_LMAGENTA 0xD
#define VGA_LBROWN   0xE
#define VGA_WHITE    0xF

// How many ascii chars I can fit
#define VGA_BUFFER_WIDTH  80
#define VGA_BUFFER_HEIGHT 25

#define VGA_MSG_SIZE 5
#define VGA_MSG_PER_EVENT (EVENT_DATA_SIZE / VGA_MSG_SIZE)

#define VGA_CLR_SCREEN_CHAR (u8) 1

typedef struct
{
    u8 row;
    u8 col;
    u8 fg;
    u8 bg;
    u8 c;
} vga_msg_t;

#endif