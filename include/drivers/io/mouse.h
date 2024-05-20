//
// Created by kamil on 05.11.23.
//

#ifndef __CPPOS__DRIVERS__IO__MOUSE_H
#define __CPPOS__DRIVERS__IO__MOUSE_H

#include <drivers/driver.h>
#include <hdcom/port.h>
#include <drivers/io/event_handler.h>
#include <hdcom/interrupts.h>

namespace cpp_os {
    namespace drivers {
        namespace io {

            class MouseEventHandler : public cpp_os::drivers::io::EventHandler {
                cpp_os::common::int8_t x, y;
            public:
                MouseEventHandler();

                ~MouseEventHandler();

                void OnKeyDown(cpp_os::common::uint8_t button);

                void OnKeyUp(cpp_os::common::uint8_t button);

                void OnMouseMove(int dx, int dy);
            };

            class MouseDriver : public cpp_os::hdcom::InterruptHandler, public cpp_os::drivers::Driver {
                cpp_os::hdcom::port8_t data_port;
                cpp_os::hdcom::port8_t command_port;

                cpp_os::common::uint8_t buffer[3];
                cpp_os::common::uint8_t offset;
                cpp_os::common::uint8_t buttons;

                cpp_os::drivers::io::MouseEventHandler *event_handler;
            public:
                MouseDriver(cpp_os::hdcom::InterruptManager *IM, cpp_os::drivers::io::MouseEventHandler *event_handler);

                ~MouseDriver();

                virtual cpp_os::common::uint32_t HandleInterrupt(cpp_os::common::uint32_t esp);

                virtual void Activate();
            };
        }
    }
}

#endif