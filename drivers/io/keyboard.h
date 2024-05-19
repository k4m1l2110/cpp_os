//
// Created by kamil on 05.11.23.
//

#ifndef CPP_OS_KEYBOARD_H
#define CPP_OS_KEYBOARD_H

#include "../driver.h"
#include "event_handler.h"
#include "../../types.h"
#include "../../interrupts.h"
#include "../../port.h"

class PrintKeyboardEventHandler : public EventHandler {
public:
    void OnKeyDown(char c);
};

class KeyboardDriver : public InterruptHandler, public Driver {
    port8_t data_port;
    port8_t command_port;

    EventHandler *event_handler;
public:
    KeyboardDriver(InterruptManager *IM, EventHandler *event_handler);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};


#endif //CPP_OS_KEYBOARD_H
