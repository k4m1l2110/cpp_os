# Makefile for building a 64-bit kernel

# Compiler and linker settings
CC = gcc
AS = as
LD = ld
CFLAGS = -m32 -c
LDFLAGS = -melf_i386

# Source files
SRCS = kernel.cpp loader.s
OBJS = kernel.o loader.o

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

loader.o: loader.s
	as --32 -o $@ $<

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO_IMAGE)

.PHONY: clean
