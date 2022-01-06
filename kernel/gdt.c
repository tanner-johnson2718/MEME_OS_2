#include "gdt.h"
#include "log.h"

// See gdt.h for documentation

///////////////////////////////////////////////////////////////////////////////
// GDT Private MACROS and defines
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// GDT entry access parameter bit fields
///////////////////////////////////////////////////////////

// Access Byte [7] - Present bit. 
#define GDT_SEGMENT_PRESENT_SHIFT 7
#define GDT_SEGMENT_PRESENT       0x1
#define GDT_SEGMENT_NOT_PRESENT   0x0

// Access Byte [5-6] - Privilege level.
#define GDT_DPL_SHIFT             5
#define GDT_DPL_KERNEL            0x0
#define GDT_DPL_USER              0x3

// Descriptor type [4] - System or Code/Data segment
#define GDT_DESC_TYPE_SHIFT       4
#define GDT_DESC_TYPE_SYS         0x0
#define GDT_DESC_TYPE_NON_SYS     0x1

// Executable bit [3] - Is segment executable
#define GDT_EXE_SHIFT            3
#define GDT_IS_EXE               0x1
#define GDT_NOT_EXE              0x0

// Direction bit/Conforming bit [2]
// Data selectors: 0) the segment grows up. 1) the segment grows down
// Code selectors: 0) code in this segment can only be executed from the ring set in Privl.
//                 1) code in this segment can be executed from an equal or lower privilege level
#define GDT_DIRECTION_SHIFT      2
#define GDT_CONFORMING_SHIFT     2
#define GDT_DIRECTION_UP         0x0
#define GDT_DIRECTION_DOWN       0x1
#define GDT_CONFORMING_NO        0x0
#define GDT_CONFORMING_YES       0x1

// R/W bit [1]
// Data selectors: 0) No write. 1) Can Write. Read always allowed
// Code selectors: 0) No read.  1) Can read.  Write never allowed
#define GDT_RW_SHIFT             1
#define GDT_SET_RW               0x1
#define GDT_NO_RW                0x0

// Accessed Bit [0] - cpu sets this
#define GDT_ACCESSED_SHIFT       0
#define GDT_ACCESSED             0x1
#define GDT_NOT_ACCESSED         0x0

///////////////////////////////////////////////////////////
// GDT entry granularity parameter bit fields
///////////////////////////////////////////////////////////

// Granularity Bit [7] - scaling factor for limit value
#define GDT_GRANULARITY_SHIFT    7
#define GDT_4K_GRAN              0x1
#define GDT_1b_GRAN              0x0

// Operand size [6]
#define GDT_OP_SIZE_SHIFT        6
#define GDT_16b_OP_SIZE          0x0
#define GDT_32b_OP_SIZE          0x1

// Bits 5-6 always 0

// Limit upper 4 bits
#define GDT_LIMIT_UPPER_SHFT     0

// A GDT entry defines a memory segment w/ the following fields
//    limit_low   - Lower 16 bits giving size of mem region
//    base_low    - lower 16 bits giving starting addr of mem region
//    base_middle - middle 8 bits of base addr
//    access      - see above
//    granularity - see above
//    base_high   - upper 8 bits of memory address
struct gdt_entry
{
    u16 limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 granularity;
    u8 base_high;
} __attribute__((packed));

// A GDT pointer. This is the data structure that is used by the lgdt and sgdt
// instructions to register / return the location of the GDT in memory.
struct gdt_ptr
{
    u16 size;
    u32 base;
} __attribute__((packed));

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

struct gdt_entry gdt[3];
struct gdt_ptr gp;

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

void gdt_flush()
{
    asm volatile("lgdt %0" :: "m" (gp));
}

void gdt_get_curr_gdt_ptr(struct gdt_ptr *p)
{

    asm volatile("sgdt (%0)" : "=a"(p):);
}

// input assumed to be good as only internal calls with hardcoded inputs
void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     gdt_install

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always a success

COMMENTS) NONE
******************************************************************************/
u8 gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.size = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (u32) &gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* We are just gonna set the entire 4GiB in the same code and data segment.
       Sounds as if the GDT isn't really used anymore. */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0x4F);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0x4F);

    // Do the same for userpace code
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0x4F);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0x4F);

    /* Flush out the old GDT and install the new changes! */
    gdt_flush();

    log_msg(__FILE__, __LINE__, "GDT installed");

    return 0;
}