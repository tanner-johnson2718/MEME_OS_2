#ifndef MEME_PS2
#define MEME_PS2

#include "types.h"

#define PS2_DATA_PORT 0x60
#define PS2_STAT_PORT 0x64   // read
#define PS2_CMND_PORT 0x64   // write

// Bit map breakout for stat register
#define PS2_OUT_BUFF_SHIFT 0x0
#define PS2_OUT_BUFF_EMPTY 0x0
#define PS2_OUT_BUFF_FULL  0x1

#define PS2_IN_BUFF_SHIFT 0x1
#define PS2_IN_BUFF_EMPTY 0x0
#define PS2_IN_BUFF_FULL  0x1

#define PS2_SYS_FLAG_SHIFT 0x2    // for bios

#define PS2_CMD_SHIFT  0x3
#define PS2_CMD_DEVICE 0x0
#define PS2_CMD_CTLR   0x1

#define PS2_TIMEOUT_ERROR 0x6
#define PS2_TIMEOUT       0x1
#define PS2_NO_TIMEMOUT   0x0

#define PS2_PARITY_ERROR_SHIFT 0x7
#define PS2_PARITY_ERROR       0x1
#define PS2_PARITY_NO_ERROR    0x0

// Macros to parse kb input
#define PS2_KEYBOARD_RELEASE_MASK 0x80
#define PS2_KEYBOARD_LSHIFT_PRESS 0x2A
#define PS2_KEYBOARD_RSHIFT_PRESS 0x36
#define PS2_KEYBOARD_LSHIFT_RELES 0xAA
#define PS2_KEYBOARD_RSHIFT_RELES 0xB6
#define PS2_KEYBOARD_CAPSLOCK     0x3A

// Bit 0 - Caps lock on?
// Bit 1 - Shift held?
// Bit 2 - Ouput caps?
struct keyboard_state
{
    u8 state;
};

#define PS2_KEYBOARD_STATE_CAPSL_SHIFT    0
#define PS2_KEYBOARD_STATE_SHIFT_HELD_SHIFT 1
#define PS2_KEYBOARD_STATE_OUT_CAPS         2
#define PS2_KEYBOARD_STATE_SHIFT_CAPSL_MASK 0xfe
#define PS2_KEYBOARD_STATE_SHIFT_RELE_MASK  0xfd
#define PS2_KEYBOARD_STATE_CAPS_OFF_MASK    0xfb


#define KEY_BOARD_STATE

// init the ps2 module and register device handlers
void ps2_init(void);

#endif