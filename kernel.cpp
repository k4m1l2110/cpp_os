//
// Created by kamil on 21.10.23.
//
#include "types.h"
#include "gdt.h"
#include "port.h"
#include "interrupts.h"
#include "./drivers/io/keyboard.h"
#include "./drivers/io/mouse.h"
#include "./drivers/driver.h"

extern "C" void __stack_chk_fail_local() {
    // Add your custom error handling here
    // For example, you can print an error message and halt the system.
    //Print("Stack buffer overflow detected\n");
    //while (1) {}  // Halt the system
    ;
}

void Print(char *str) {
    static uint16_t *VideoMemory = (uint16_t *) 0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        switch(str[i]){
            case '\n':
                y++;
                x=0;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
        }

        if (x >= 80) {
            y++;
            x = 0;
        }
        if (y >= 25) {
            for(y=0;y<25;y++)
                for(x=0;x<80;x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            y = 0;
            x = 0;
        }
    }
}

void PrintHex(uint8_t key) {
    char *res = "00";
    char *hex = "0123456789ABCDEF";
    res[0] = hex[(key >> 4) & 0x0F];
    res[1] = hex[key & 0x0F];
    Print(res);
}

typedef void (*constructor)();

extern "C" constructor start_ctors;

extern "C" constructor end_ctors;

extern "C" void CallConstructors() {
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void KernelMain(const void *multiboot_structure, uint32_t magic) {
    Print("Hello World!\n");

    GDT _gdt;

    InterruptManager _interrupts(&_gdt);
    DriverManager _drivers;

    Print("Initializing drivers...\n");

    PrintKeyboardEventHandler _kb_handler;

    KeyboardDriver keyboard(&_interrupts, &_kb_handler);
    _drivers.AddDriver(&keyboard);

    MouseEventHandler _mouse_handler;
    MouseDriver mouse(&_interrupts, &_mouse_handler);
    _drivers.AddDriver(&mouse);

    _drivers.ActivateAll();

    _interrupts.Activate();

    while (1);
}