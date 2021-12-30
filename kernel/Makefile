CC = gcc
CFLAGS = -m32 -fno-pie -fno-stack-protector -I../include -I../include/kernel

SRC=$(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(OBJ) entry

entry: entry.S
	$(CC) $(CFLAGS) -c entry.S -o entry.o 

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o