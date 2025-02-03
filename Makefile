TARGET = i686
LD = $(TARGET)-elf-gcc

all: solkern.elf

solkern.elf: kernel cpu driver lib
	$(LD) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $(wildcard bin/*.o) -lgcc

debug: clean solkern.elf
	qemu-system-i386 -s -S -kernel solkern.elf

clean:
	rm bin/*

kernel:
	make -C src/kernel

cpu:
	make -C src/arch/$(TARGET)

driver:
	make -C src/driver

lib:
	make -C src/lib
