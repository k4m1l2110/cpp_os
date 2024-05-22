# Compiler and linker settings
CC = gcc
AS = as
LD = ld

CFLAGS = -m32 -Iinclude -c -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
LDFLAGS = -melf_i386

CPP_SOURCES = $(wildcard src/**/**/*.cpp src/**/*.cpp src/*.cpp)
S_SOURCES = $(wildcard src/**/*.s src/*.s)

OBJS = $(patsubst src/%.cpp,obj/%.o,$(CPP_SOURCES)) $(patsubst src/%.s,obj/%.o,$(S_SOURCES))

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

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: src/%.s
	mkdir -p $(@D)
	as --32 -o $@ $<

run: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'cpp_os' &

clean:
	rm -Rf obj $(KERNEL) $(ISO_IMAGE) iso

.PHONY: clean