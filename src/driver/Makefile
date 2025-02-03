MODULE = driver
INCLUDE = ../../include
BINDIR = ../../bin

ASM = nasm
CC  = i686-elf-gcc

AFLAGS = -g -felf -w+gnu-elf-extensions
CFLAGS = -g -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(INCLUDE)

C_SRC = $(wildcard *.c)
C_OBJ = $(C_SRC:%.c=$(BINDIR)/$(MODULE)_%.o)

A_SRC = $(wildcard *.asm)
A_OBJ = $(A_SRC:%.asm=$(BINDIR)/$(MODULE)a_%.o)

all: $(C_OBJ) $(A_OBJ)

$(BINDIR)/$(MODULE)_%.o: %.c
	$(CC) -o $@ $< $(CFLAGS)

$(BINDIR)/$(MODULE)a_%.o: %.asm
	$(ASM) $(AFLAGS) -o $@ $<
