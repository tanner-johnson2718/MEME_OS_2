# Entry point of the MEME OS 2 EB

.global start

/* Multi boot specification needs this magic number with in the first 8192 
   bytes of first sector on bootable disk */
.equ MAGIC_NUMBER, 0x1BADB002      
.equ FLAGS, 0x0                    # No flags?
.equ CHECKSUM, -MAGIC_NUMBER       # Apparently CHECKUM + FLAGS + MAGIC NUMBER = 0?

    .text
.align 4

    .long MAGIC_NUMBER
    .long FLAGS
    .long CHECKSUM

start:

loop:
    mov $826, %eax
    call kernel_main