#GCCPARAMS = -m32 -fno-use-cxa-ateix -nostdlib -no-builtin 0fno-exceptions -fno-leading
GPPPARAMS = -m32
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o

%.o: %.cpp
		g++ $(GPPPARMAS) -o $@ -c $<
%.o: %.s
		as $(ASPARAMS) -o $@ $<

my_kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: my_kernel.bin
		sudo cp $< /boot/my_kernel.bin
