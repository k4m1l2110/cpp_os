//
// Created by kamil on 01.11.23.
//

#include <hdcom/interrupts.h>

void Print(char *str);
void PrintHex(cpp_os::common::uint8_t key);

cpp_os::hdcom::InterruptManager *cpp_os::hdcom::InterruptManager::active_IM = 0;
cpp_os::hdcom::InterruptManager::GateDescriptor cpp_os::hdcom::InterruptManager::IDT[256];

cpp_os::hdcom::InterruptHandler::InterruptHandler(cpp_os::common::uint8_t interruptnr, cpp_os::hdcom::InterruptManager *im)
        : interrupt_number(interruptnr), IM(im) {
    IM->handlers[interrupt_number] = this;

}

cpp_os::hdcom::InterruptHandler::~InterruptHandler() {
    if (IM->handlers[interrupt_number] == this)
        IM->handlers[interrupt_number] = 0;
}

cpp_os::common::uint32_t cpp_os::hdcom::InterruptHandler::HandleInterrupt(cpp_os::common::uint32_t esp) {
    return esp;
}

cpp_os::hdcom::InterruptManager::InterruptManager(GDT *gdt)
        : pic_master_command(0x20), pic_master_data(0x21),
          pic_slave_command(0xA0), pic_slave_data(0xA1) {


cpp_os::common::uint16_t code_segment = gdt->CodeSegmentSelector();
    const cpp_os::common::uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (cpp_os::common::uint16_t i = 0; i < 256; i++) {
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
    _idt.base = (cpp_os::common::uint32_t) IDT;
    asm volatile("lidt %0" : : "m"(_idt));

}

void cpp_os::hdcom::InterruptManager::SetIDT(cpp_os::common::uint8_t interrupt_number,
                                             cpp_os::common::uint16_t code_segment_selector_offset,
                              void (*handler)(),
                              cpp_os::common::uint8_t descriptor_privilage_level,
                              cpp_os::common::uint8_t descriptor_type) {

    const cpp_os::common::uint8_t IDT_DESC_PRESENT = 0x80;

    IDT[interrupt_number].handle_address_low_bits = ((cpp_os::common::uint32_t) handler) & 0xFFFF;
    IDT[interrupt_number].handle_address_high_bits = ((cpp_os::common::uint32_t) handler >> 16) & 0xFFFF;
    IDT[interrupt_number].GDT_code_segment_selector = code_segment_selector_offset;
    IDT[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilage_level & 3) << 5);
    IDT[interrupt_number].reserved = 0;
}

cpp_os::common::uint32_t cpp_os::hdcom::InterruptManager::DoHandleInterrupt(cpp_os::common::uint8_t interruptnumber, cpp_os::common::uint32_t esp) {

    if(handlers[interruptnumber]!= 0)
        esp = handlers[interruptnumber]->HandleInterrupt(esp);
    else if (interruptnumber != 0x20) {
        Print("UNHANDLED INTERRUPT 0x");
        PrintHex(interruptnumber);
    }

    if (0x20 <= interruptnumber && 0x30 > interruptnumber) {
        pic_master_command.Write(0x20);
        if (0x28 <= interruptnumber)
            pic_slave_command.Write(0x20);
    }
    return esp;
}

cpp_os::common::uint32_t cpp_os::hdcom::InterruptManager::HandleInterrupt(cpp_os::common::uint8_t interruptNumber, cpp_os::common::uint32_t esp) {
    if (active_IM != nullptr)
        return active_IM->DoHandleInterrupt(interruptNumber, esp);
    return esp;
}

void cpp_os::hdcom::InterruptManager::Activate() {
    if (active_IM != nullptr)
        active_IM->Deactivate();
    active_IM = this;
    asm("sti");
    Print("Activate\n");
}

void cpp_os::hdcom::InterruptManager::Deactivate() {
    if (active_IM == this) {
        active_IM = nullptr;
        asm("cli");
        Print("Dectivate\n");
    }
}