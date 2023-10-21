//
// Created by kamil on 21.10.23.
//

void printf(char *str) {
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for (int i = 0; str[i] != '\0'; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

typedef void (*constructor)();

extern "C" constructor start_ctors;

extern "C" constructor end_ctors;

extern "C" void CallConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++) // Note the '&' before end_ctors
        (*i)();
}

extern "C" void KernelMain(const void* multiboot_structure, unsigned int) {
    printf("Hello World!");
    while (1)
        ;
}
