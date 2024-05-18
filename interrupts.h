//
// Created by kamil on 01.11.23.
//

#ifndef INTERUPTS_H
#define INTERUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager;

class InterruptHandler{
protected:
    uint8_t interrupt_number;
    InterruptManager* IM;
    InterruptHandler(uint8_t interruptnr, InterruptManager* IM);
    ~InterruptHandler();
public:
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager {
    friend class InterruptHandler;
protected:
    static InterruptManager *active_IM;
    InterruptHandler* handlers[256];

    struct GateDescriptor{
        uint16_t handle_address_low_bits;
        uint16_t GDT_code_segment_selector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handle_address_high_bits;
    } __attribute__((packed));

    //Interrupt Descriptor Table
    static GateDescriptor IDT[256];

    struct IDT_PTR{
        uint16_t size;
        uint32_t  base;
    } __attribute__((packed));

    static void SetIDT(uint8_t interrupt_number,
                       uint16_t code_segment_selector_offset,
                       void (*handler)(),
                       uint8_t descriptor_privilage_level,
                       uint8_t descriptor_type);

    port8_t_slow pic_master_command;
    port8_t_slow pic_master_data;
    port8_t_slow pic_slave_command;
    port8_t_slow pic_slave_data;

    static uint32_t HandleInterrupt(uint8_t interrupt_number, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t interrupt_number, uint32_t esp);
    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x0C();

public:
    InterruptManager(GDT *gdt);
    ~InterruptManager(){;}
    void Activate();
    void Deactivate();


};


#endif //INTERUPTS_H
