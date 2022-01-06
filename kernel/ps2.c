#include "ps2.h"
#include "types.h"
#include "io_port.h"
#include "irq.h"
#include "log.h"

///////////////////////////////////////////////////////////////////////////////
// Private defines, macros and types
///////////////////////////////////////////////////////////////////////////////

// ASCII map to trasnlate keys to ascii text
u8 key_ascii_map[128] = 
{
    0,  27,                                 // escape
    '1', '2', '3', '4', '5', '6', '7', '8',	// 9 
    '9', '0', '-', '=', '\b',	            // Backspace
    '\t',			                        // Tab 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 
    'o', 'p', '[', ']', '\n',	            // Enter key
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

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
// Bit 3 - CTRL held?
// Bit 4 - Alt held?
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

///////////////////////////////////////////////////////////////////////////////
// Internal State
///////////////////////////////////////////////////////////////////////////////

// Keyboard state
struct keyboard_state state = {0};

// Registered input handler
void (*ps2_internal_handler)(u8) = {0};

///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void update_caps_state()
{
    u8 shift_held = (state.state >> PS2_KEYBOARD_STATE_SHIFT_HELD_SHIFT) & 0x1;
    u8 caps_on = (state.state >> PS2_KEYBOARD_STATE_CAPSL_SHIFT) & 0x1;
    u8 caps = caps_on ^ shift_held;

    if(caps){
        state.state |=  (0x1 << PS2_KEYBOARD_STATE_OUT_CAPS);
    }
    else
    {
        state.state &= PS2_KEYBOARD_STATE_CAPS_OFF_MASK;
    }
    
}

u8 is_alphabet_char(u8 in_ascii)
{
    if(in_ascii >= 0x61 && in_ascii <= 0x7A)
    {
        return 1;
    }

    return 0;
}

// Takes into account caps state of keyboard
u8 convert_to_ascii(u8 in)
{
    // map input to actual ascii and output
    u8 in_ascii = key_ascii_map[in];

    // output caps if caps bit set in state and output char is a-z
    if(( (state.state >> PS2_KEYBOARD_STATE_OUT_CAPS) & 0x1) && is_alphabet_char(in_ascii))
    {
        in_ascii -= 0x20;
    }

    return in_ascii;
}

// If top bit on, then release event
u8 is_release_event(u8 in)
{
    return in & PS2_KEYBOARD_RELEASE_MASK;
}

void ps2_keyboard_irq(void)
{
    u8 in = inb(PS2_DATA_PORT);

    // Check if key released
    if( is_release_event(in) )
    {
        // key released event

        // Check if shift released
        if(in == PS2_KEYBOARD_LSHIFT_RELES || in == PS2_KEYBOARD_RSHIFT_RELES)
        {
            // update shfit held bit in state
            state.state &= PS2_KEYBOARD_STATE_SHIFT_RELE_MASK;
        }

        update_caps_state();
    }
    else
    {
        // key pressed event

        // Check if shift pressed
        if(in == PS2_KEYBOARD_LSHIFT_PRESS || in == PS2_KEYBOARD_RSHIFT_PRESS)
        {
            // update shfit held bit in state
            state.state |= (0x1 << PS2_KEYBOARD_STATE_SHIFT_HELD_SHIFT);
        }

        // check if caps lock pressed
        if(in == PS2_KEYBOARD_CAPSLOCK)
        {
            u8 caps_on = (state.state >> PS2_KEYBOARD_STATE_CAPSL_SHIFT) & 0x1;

            if(!caps_on){
                state.state |=  (0x1 << PS2_KEYBOARD_STATE_CAPSL_SHIFT);
            }
            else
            {
                state.state &= PS2_KEYBOARD_STATE_SHIFT_CAPSL_MASK;
            }
        }

        update_caps_state();
        u8 ascii_in = convert_to_ascii(in);

        if(ps2_internal_handler)
        {
            ps2_internal_handler(ascii_in);
        }
        else
        {
            log_msg(__FILE__,__LINE__,"PS2 IRQ recieved but no handler is registered");
        }
        
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////


/******************************************************************************
NAME)     ps2_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 on success, otherwise error code

COMMENTS) NONE
******************************************************************************/
u8 ps2_init(void)
{
    if( irq_register_PIC_handler(ps2_keyboard_irq, IRQ_PIC_KEY) )
    {
        log_msg(__FILE__,__LINE__,"Failed to register PS2 IRQ handler");
        return PS2_IRQ_REG_FAILED;
    }

    log_msg(__FILE__,__LINE__,"PS2 successfully init-ed");
    return 0;
}



/******************************************************************************
NAME)     ps2_register_handler

INPUTS)   
          0) handler - function pointer that takes ascii byte as input

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 ps2_register_hanlder(void (*handler)(u8))
{
    ps2_internal_handler = handler;
    return 0;
}