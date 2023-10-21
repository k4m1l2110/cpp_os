#GCCPARAMS = -m32 -fno-use-cxa-ateix -nostdlib -no-builtin 0fno-exceptions -fno-leading
ASPARAMS = --64
# Compilation parameters for 64-bit AMD
GPPPARAMS = -m64 -fno-use-cxa-atexit -nostdlib -fno-exceptions

# Linking parameters for 64-bit AMD
LDPARAMS = -melf_x86_64

objects = loader.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as --64 -o $@ $<

my_kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: my_kernel.bin
	sudo cp $< /boot/my_kernel.bin