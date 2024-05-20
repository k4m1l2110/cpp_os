//
// Created by kamil on 01.11.23.
//

#ifndef __CPPOS__HDCOM__INTERUPTS_H
#define __CPPOS__HDCOM__INTERUPTS_H

#include <common/types.h>
#include <hdcom/port.h>
#include <gdt.h>

namespace cpp_os {
    namespace hdcom {

        class InterruptManager;

        class InterruptHandler {
        protected:
            cpp_os::common::uint8_t interrupt_number;
            InterruptManager *IM;

            InterruptHandler(cpp_os::common::uint8_t interruptnr, InterruptManager *IM);

            ~InterruptHandler();

        public:
            virtual cpp_os::common::uint32_t HandleInterrupt(cpp_os::common::uint32_t esp);
        };

        class InterruptManager {
            friend class InterruptHandler;

        protected:
            static InterruptManager *active_IM;
            InterruptHandler *handlers[256];

            struct GateDescriptor {
                cpp_os::common::uint16_t handle_address_low_bits;
                cpp_os::common::uint16_t GDT_code_segment_selector;
                cpp_os::common::uint8_t reserved;
                cpp_os::common::uint8_t access;
                cpp_os::common::uint16_t handle_address_high_bits;
            } __attribute__((packed));

            //Interrupt Descriptor Table
            static GateDescriptor IDT[256];

            struct IDT_PTR {
                cpp_os::common::uint16_t size;
                cpp_os::common::uint32_t base;
            } __attribute__((packed));

            static void SetIDT(cpp_os::common::uint8_t interrupt_number,
                               cpp_os::common::uint16_t code_segment_selector_offset,
                               void (*handler)(),
                               cpp_os::common::uint8_t descriptor_privilage_level,
                               cpp_os::common::uint8_t descriptor_type);

            cpp_os::hdcom::port8_t_slow pic_master_command;
            cpp_os::hdcom::port8_t_slow pic_master_data;
            cpp_os::hdcom::port8_t_slow pic_slave_command;
            cpp_os::hdcom::port8_t_slow pic_slave_data;

            static cpp_os::common::uint32_t HandleInterrupt(cpp_os::common::uint8_t interrupt_number, cpp_os::common::uint32_t esp);

            cpp_os::common::uint32_t DoHandleInterrupt(cpp_os::common::uint8_t interrupt_number, cpp_os::common::uint32_t esp);

            static void IgnoreInterruptRequest();

            static void HandleInterruptRequest0x00();

            static void HandleInterruptRequest0x01();

            static void HandleInterruptRequest0x0C();

        public:
            InterruptManager(GDT *gdt);

            ~InterruptManager() { ; }

            void Activate();

            void Deactivate();


        };
    }
}

#endif
