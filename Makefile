TARGET = i686
LD = $(TARGET)-elf-gcc

all: diskimg

diskimg: solkern.elf
	dd if=/dev/zero of=disk.img bs=1M count=100
	printf 'o\nn\np\n1\n\n\nw\n' | fdisk disk.img
	sudo kpartx -av disk.img
	sudo mkfs.fat -F32 /dev/mapper/loop0p1
	sudo mount -t vfat /dev/mapper/loop0p1 sysroot
	sudo mkdir -p sysroot/boot/solkern
	sudo cp solkern.elf sysroot/boot/solkern/solkern
	sudo cp limine.conf sysroot/boot/limine.conf
	sudo cp /usr/share/limine/limine-bios.sys sysroot/boot/
	sudo limine bios-install disk.img
	sudo umount sysroot
	sudo kpartx -dv disk.img

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
