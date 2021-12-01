CC = gcc

WARNINGS = -Wall -W -Wstrict-prototypes -Wmissing-prototypes -Wsystem-headers
CFLAGS = -g -msoft-float -O -fno-stack-protector
CPPFLAGS = -nostdinc -I. -Ilib -Ilib/kernel 
ASFLAGS = -Wa,--gstabs
LDFLAGS = -T link.ld -melf_i386
DEPS = -MMD -MF $(@:.o=.d)

all: kernel.elf

SOURCES = entry.S
OBJECTS = entry.o
DEPENDS = entry.d

%.o: %.S
	$(CC) -m32 -c $< -o $@ $(ASFLAGS) $(CPPFLAGS) $(DEFINES) $(DEPS)

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

clean:
	rm -f $(OBJECTS) $(DEPENDS) 
	rm kernel.elf