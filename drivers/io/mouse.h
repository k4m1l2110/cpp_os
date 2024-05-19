//
// Created by kamil on 05.11.23.
//

#ifndef CPP_OS_MOUSE_H
#define CPP_OS_MOUSE_H

#include "../driver.h"
#include "event_handler.h"
#include "../../types.h"
#include "../../interrupts.h"
#include "../../port.h"

class MouseEventHandler :public EventHandler{
    int8_t x, y;
public:
    MouseEventHandler();
    ~MouseEventHandler();
    void OnKeyDown(uint8_t button);
    void OnKeyUp(uint8_t button);
    void OnMouseMove(int dx, int dy);
};

class MouseDriver : public InterruptHandler, public Driver {
    port8_t data_port;
    port8_t command_port;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    MouseEventHandler *event_handler;
public:
    MouseDriver(InterruptManager *IM, MouseEventHandler *event_handler);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};


#endif //CPP_OS_MOUSE_H
