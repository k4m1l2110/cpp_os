//
// Created by kamil on 01.11.23.
//

#include "interrupts.h"

void Print(char *str);

InterruptManager *InterruptManager::active_IM = 0;
InterruptManager::GateDescriptor InterruptManager::IDT[256];

InterruptHandler::InterruptHandler(uint8_t interruptnr, InterruptManager *im)
        : interrupt_number(interruptnr), IM(im) {
    IM->handlers[interrupt_number] = this;

}

InterruptHandler::~InterruptHandler() {
    if (IM->handlers[interrupt_number] == this)
        IM->handlers[interrupt_number] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}

InterruptManager::InterruptManager(GDT *gdt)
        : pic_master_command(0x20), pic_master_data(0x21),
          pic_slave_command(0xA0), pic_slave_data(0xA1) {


    uint16_t code_segment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++) {
        handlers[i] = 0;
        SetIDT(i, code_segment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    SetIDT(0x20, code_segment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetIDT(0x21, code_segment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetIDT(0x2C, code_segment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

    pic_master_command.Write(0x11);
    pic_slave_command.Write(0x11);

    pic_master_data.Write(0x20);
    pic_slave_data.Write(0x28);

    pic_master_data.Write(0x04);
    pic_slave_data.Write(0x02);

    pic_master_data.Write(0x01);
    pic_slave_data.Write(0x01);

    pic_master_data.Write(0x00);
    pic_slave_data.Write(0x00);

    IDT_PTR _idt;
    _idt.size = 256 * sizeof(GateDescriptor) - 1;
    _idt.base = (uint32_t) IDT;
    asm volatile("lidt %0" : : "m"(_idt));

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

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptnumber, uint32_t esp) {

    if(handlers[interruptnumber]!= 0)
        esp = handlers[interruptnumber]->HandleInterrupt(esp);
    else if (interruptnumber != 0x20) {
        char *msg = "Unhandled interrupt 0x00 ";
        char *hex = "0123456789ABCDEF";
        msg[22] = hex[(interruptnumber >> 4) & 0x0F];
        msg[23] = hex[interruptnumber & 0x0F];
        Print(msg);
    }

    if (0x20 <= interruptnumber && 0x30 > interruptnumber) {
        pic_master_command.Write(0x20);
        if (0x28 <= interruptnumber)
            pic_slave_command.Write(0x20);
    }
    return esp;
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (active_IM != nullptr)
        return active_IM->DoHandleInterrupt(interruptNumber, esp);
    return esp;
}

void InterruptManager::Activate() {
    if (active_IM != nullptr)
        active_IM->Deactivate();
    active_IM = this;
    asm("sti");
    Print("Activate\n");
}

void InterruptManager::Deactivate() {
    if (active_IM == this) {
        active_IM = nullptr;
        asm("cli");
        Print("Dectivate\n");
    }
}