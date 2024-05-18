//
// Created by kamil on 05.11.23.
//

#ifndef CPP_OS_MOUSE_H
#define CPP_OS_MOUSE_H

#include "../types.h"
#include "../interrupts.h"
#include "../port.h"

class MouseDriver : public InterruptHandler {
    port8_t data_port;
    port8_t command_port;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
public:
    MouseDriver(InterruptManager *IM);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};


#endif //CPP_OS_MOUSE_H
