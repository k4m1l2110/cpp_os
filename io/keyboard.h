//
// Created by kamil on 05.11.23.
//

#ifndef CPP_OS_KEYBOARD_H
#define CPP_OS_KEYBOARD_H

#include "../types.h"
#include "../interrupts.h"
#include "../port.h"

class KeyboardDriver : public InterruptHandler {
    port8_t data_port;
    port8_t command_port;
public:
    KeyboardDriver(InterruptManager *IM);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};


#endif //CPP_OS_KEYBOARD_H
