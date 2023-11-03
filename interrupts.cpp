//
// Created by kamil on 01.11.23.
//

#include "interrupts.h"

void Print(char *str);

InterruptManager::GateDescriptor InterruptManager::IDT[256];

InterruptManager::InterruptManager(GDT *gdt)
        : pic_master_command(0x20), pic_master_data(0x21),
          pic_slave_command(0xA0), pic_slave_data(0xA1) {


    uint16_t code_segment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++) {
        SetIDT(i, code_segment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    SetIDT(0x20, code_segment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetIDT(0x21, code_segment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    pic_master_command.Write(0x11);
    pic_slave_command.Write(0x11);

    pic_master_data.Write(0x20);
    pic_slave_data.Write(0x28);

    IDT_PTR _IDT;
    _IDT.size = 256 * sizeof(GateDescriptor) - 1;
    _IDT.base = (uint32_t) IDT;

    asm volatile("lidt %0" : : "m"(_IDT));

}



uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    Print("Interrupt\n");
    return esp;
}

void InterruptManager::SetIDT(uint8_t interrupt_number,
                              uint16_t code_segment_selector_offset,
                              void (*handler)(),
                              uint8_t descriptor_privilage_level,
                              uint8_t descriptor_type) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    IDT[interrupt_number].handle_address_low_bits = ((uint32_t) handler) & 0xFFFF;
    IDT[interrupt_number].handle_address_high_bits = ((uint32_t) handler >> 16) & 0xFFFF;
    IDT[interrupt_number].GDT_code_segment_selector = code_segment_selector_offset;
    IDT[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilage_level & 3) << 5);
    IDT[interrupt_number].reserved = 0;
}

void InterruptManager::Activate() {
    asm("sti");
    Print("Interrupt\n");
}