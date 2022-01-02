#ifndef MEME_VGA_MSG
#define MEME_VGA_MSG

#include "types.h"
#include "sched_app.h"



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