//
// Created by kamil on 21.10.23.
//
#include <common/types.h>
#include <gdt.h>
#include <hdcom/pci.h>
#include <hdcom/interrupts.h>
#include <drivers/io/keyboard.h>
#include <drivers/io/mouse.h>
#include <drivers/driver.h>



extern "C" void __stack_chk_fail_local() {
    // Add your custom error handling here
    // For example, you can print an error message and halt the system.
    //Print("Stack buffer overflow detected\n");
    //while (1) {}  // Halt the system
    ;
}

void Print(char *str) {
    static cpp_os::common::uint16_t *VideoMemory = (cpp_os::common::uint16_t *) 0xb8000;

    static cpp_os::common::uint8_t x = 0, y = 0;

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

void PrintHex(cpp_os::common::uint8_t key) {
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

extern "C" void KernelMain(const void *multiboot_structure, cpp_os::common::uint32_t magic) {
    Print("Hello World!\n");

    //Create a Global Descriptor Table
    cpp_os::GDT _gdt;


    //Create an Interrupt Manager
    cpp_os::hdcom::InterruptManager _interrupts(&_gdt);
    //Select drivers
    cpp_os::drivers::DriverManager _drivers;

    Print("Initializing drivers...\n");

    cpp_os::drivers::io::PrintKeyboardEventHandler _kb_handler;

    cpp_os::drivers::io::KeyboardDriver keyboard(&_interrupts, &_kb_handler);
    _drivers.AddDriver(&keyboard);

    cpp_os::drivers::io::MouseEventHandler _mouse_handler;
    cpp_os::drivers::io::MouseDriver mouse(&_interrupts, &_mouse_handler);
    _drivers.AddDriver(&mouse);

    //Create a Peripheral Component Interconnect Controller
    cpp_os::hdcom::PCIC _pci;
    _pci.SelectDrivers(&_drivers, &_interrupts);

    _drivers.ActivateAll();

    _interrupts.Activate();

    while (1);
}