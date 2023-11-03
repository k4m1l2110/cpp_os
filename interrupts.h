//
// Created by kamil on 01.11.23.
//

#ifndef INTERUPTS_H
#define INTERUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
protected:
    struct GateDescriptor{
        uint16_t handle_address_low_bits;
        uint16_t GDT_code_segment_selector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handle_address_high_bits;
    } __attribute__((packed));

    static GateDescriptor IDT[256];//Interrupt Descriptor Table

    struct IDT_PTR{
        uint16_t size;
        uint32_t  base;
    } __attribute__((packed));

    static void SetIDT(uint8_t interrupt_number,
                       uint16_t code_segment_selector_offset,
                       void (*handler)(),
                       uint8_t descriptor_privilage_level,
                       uint8_t descriptor_type);

    Port8_t_slow pic_master_command;
    Port8_t_slow pic_master_data;
    Port8_t_slow pic_slave_command;
    Port8_t_slow pic_slave_data;

public:
    InterruptManager(GDT *gdt);

    void Activate();

    static uint32_t HandleInterrupt(uint8_t interrupt_number, uint32_t esp);
    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
};


#endif //INTERUPTS_H
