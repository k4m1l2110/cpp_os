//
// Created by kamil on 05.11.23.
//
#ifndef __CPPOS__DRIVERS__IO__KEYBOARD_H
#define __CPPOS__DRIVERS__IO__KEYBOARD_H

#include <drivers/driver.h>
#include <drivers/io/event_handler.h>
#include <hdcom/interrupts.h>
#include <common/types.h>
#include <hdcom/interrupts.h>
#include <hdcom/port.h>

namespace cpp_os {
    namespace drivers {
        namespace io {

            class PrintKeyboardEventHandler : public cpp_os::drivers::io::EventHandler {
            public:
                void OnKeyDown(char c);
            };

            class KeyboardDriver : public cpp_os::hdcom::InterruptHandler, public cpp_os::drivers::Driver {
                cpp_os::hdcom::port8_t data_port;
                cpp_os::hdcom::port8_t command_port;

                cpp_os::drivers::io::EventHandler *event_handler;
            public:
                KeyboardDriver(cpp_os::hdcom::InterruptManager *IM, cpp_os::drivers::io::EventHandler *event_handler);

                ~KeyboardDriver();

                virtual cpp_os::common::uint32_t HandleInterrupt(cpp_os::common::uint32_t esp);

                virtual void Activate();
            };
        }
    }
}

#endif
