CFLAGS=-std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra -g
CPPFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LIBS=-Iinclude -nostdlib

AS=..\TOOLS\ghost-i686-elf-tools-win32\ghost-i686-elf-tools\bin\i686-elf-as.exe
CC=..\TOOLS\ghost-i686-elf-tools-win32\ghost-i686-elf-tools\bin\i686-elf-g++.exe
MKIMG=..\TOOLS\grub-2.02-for-windows\grub-mkimage.exe
MKISO=..\TOOLS\mkisofs.exe
QEMU=C:\Program Files (x86)\qemu\qemu-system-i386.exe

PROJDIR=core_c etc

OBJFILES:=kernel.o etc/boot.o core_c/io.o etc/memcpy.o etc/memset.o vga.o gfx.o math.o matrix.o vector.o raytracer.o

all: clean myos.bin
	
%.o: %.s
	$(AS) -c $< -o $@

%.o: %.S
	$(CC) -c $< -o $@
	
%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@
	
%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $(LIBS) $< -o $@
	
myos.bin: $(OBJFILES)
	$(CC) -T etc\linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $(OBJFILES) -lgcc
	del *.o

myos.iso: myos.bin
	xcopy "..\TOOLS\grub-2.02-for-windows\i386-pc\*" "isodir\boot\grub\i386-pc\" /i
	del "isodir\boot\grub\i386-pc\*.img"
	copy "myos.bin" "isodir\boot\myos.bin"
	copy "etc\grub.cfg" "isodir\boot\grub\grub.cfg"
	$(MKIMG) -O i386-pc -d ..\TOOLS\grub-2.02-for-windows\i386-pc -o core.img -c etc\load_cfg --prefix=/boot/grub iso9660 biosdisk
	type "..\TOOLS\grub-2.02-for-windows\i386-pc\cdboot.img" "core.img" > "isodir\boot\grub\i386-pc\eltorito.img"
	$(MKISO) -graft-points -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-info-table -o myos.iso -r isodir
	rd "isodir\" /s /q
	del "core.img"
	
run: myos.bin
	$(QEMU) -kernel myos.bin

runiso: myos.iso
	$(QEMU) -cdrom myos.iso
	
clean:
	del /s /q *.o *.bin *.iso