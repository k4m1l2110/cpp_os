//
// Created by kamil on 21.10.23.
//

// kernel.cpp

#include <cstdint>

extern "C" void print(const char* str) {
    volatile uint16_t* VideoMemory = (uint16_t*)0xB8000;

    uint16_t color = 0x0F00;

    // Loop through the string
    for (int i = 0; str[i] != '\0'; ++i) {
        // Combine character and color attribute into a 16-bit value
        uint16_t character = (uint16_t)str[i] | color;

        // Write to video memory
        VideoMemory[i] = character;
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
