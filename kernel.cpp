//
// Created by kamil on 21.10.23.
//

// kernel.cpp

extern "C" void print(const char* str) {
    volatile unsigned short* VideoMemory = (unsigned short*)0xB8000;

    for (int i = 0; str[i] != '\0'; ++i) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

typedef void (*constructor)();

extern "C" constructor start_ctors;

extern "C" constructor end_ctors;

extern "C" void CallConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++) // Note the '&' before end_ctors
        (*i)();
}

extern "C" void KernelMain() {
    const char* hello = "Hello, World!";
    print(hello);

    while (1) {
        // Infinite loop to keep the kernel running
    }
}
