CC = gcc
CFLAGS = -m32 -fno-pie -fno-stack-protector -I../include -I../include/user

SRC=$(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o