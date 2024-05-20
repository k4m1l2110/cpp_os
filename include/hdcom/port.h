//
// Created by kamil on 01.11.23.
//

#ifndef __CPPOS__HDCOM__PORT_H
#define __CPPOS__HDCOM__PORT_H

#include <common/types.h>


namespace cpp_os {
    namespace hdcom {

        class port {
        protected:
            port(cpp_os::common::uint16_t portnumber);

            // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
            ~port();

            cpp_os::common::uint16_t portnumber;
        };


        class port8_t : public port {
        public:
            port8_t(cpp_os::common::uint16_t portnumber);

            ~port8_t();

            virtual cpp_os::common::uint8_t Read();

            virtual void Write(cpp_os::common::uint8_t data);

        protected:
            static inline cpp_os::common::uint8_t Read8(cpp_os::common::uint16_t _port) {
                cpp_os::common::uint8_t result;
                __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write8(cpp_os::common::uint16_t _port, cpp_os::common::uint8_t _data) {
                __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };


        class port8_t_slow : public port8_t {
        public:
            port8_t_slow(cpp_os::common::uint16_t portnumber);

            ~port8_t_slow();

            virtual void Write(cpp_os::common::uint8_t data);

        protected:
            static inline void Write8Slow(cpp_os::common::uint16_t _port, cpp_os::common::uint8_t _data) {
                __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
            }

        };


        class port16_t : public port {
        public:
            port16_t(cpp_os::common::uint16_t portnumber);

            ~port16_t();

            virtual cpp_os::common::uint16_t Read();

            virtual void Write(cpp_os::common::uint16_t data);

        protected:
            static inline cpp_os::common::uint16_t Read16(cpp_os::common::uint16_t _port) {
                cpp_os::common::uint16_t result;
                __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write16(cpp_os::common::uint16_t _port, cpp_os::common::uint16_t _data) {
                __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };


        class port32_t : public port {
        public:
            port32_t(cpp_os::common::uint16_t portnumber);

            ~port32_t();

            virtual cpp_os::common::uint32_t Read();

            virtual void Write(cpp_os::common::uint32_t data);

        protected:
            static inline cpp_os::common::uint32_t Read32(cpp_os::common::uint16_t _port) {
                cpp_os::common::uint32_t result;
                __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write32(cpp_os::common::uint16_t _port, cpp_os::common::uint32_t _data) {
                __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
            }
        };
    }
}
#endif
