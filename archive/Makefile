CC = gcc
CFLAGS = -m32 -fno-pie -fno-stack-protector -I../include

SRC=$(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o