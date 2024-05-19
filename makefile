# Compiler and linker settings
CC = gcc
AS = as
LD = ld

CFLAGS = -m32 -c -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
LDFLAGS = -melf_i386

OBJS = loader.o gdt.o port.o interruptstubs.o interrupts.o ./drivers/io/keyboard.o ./drivers/io/mouse.o ./drivers/io/event_handler.o ./drivers/driver.o kernel.o

# Output files
OUTPUT_DIR = iso/boot
KERNEL = mykernel.bin
ISO_IMAGE = mykernel.iso

all: $(ISO_IMAGE)

$(ISO_IMAGE): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) $(OUTPUT_DIR)/$(KERNEL)
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue --output=$(ISO_IMAGE) iso

$(KERNEL): $(OBJS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJS) -T linker.ld

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	as --32 -o $@ $<

run: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'cpp_os' &

clean:
	rm -Rf $(OBJS) $(KERNEL) $(ISO_IMAGE) iso

.PHONY: clean
