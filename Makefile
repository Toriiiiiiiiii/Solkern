TARGET = i686
LD = $(TARGET)-elf-gcc

all: cdrom

cdrom: solkern.elf
	rm -rf sysroot/*
	mkdir -p sysroot/boot/grub
	cp solkern.elf sysroot/boot/kernel.elf
	cp grub.cfg sysroot/boot/grub/grub.cfg
	grub-mkrescue -o cdrom.iso sysroot

solkern.elf: kernel cpu driver lib
	$(LD) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $(wildcard bin/*.o) -lgcc

debug: clean cdrom
	qemu-system-x86_64 -s -S --cdrom cdrom.iso

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
