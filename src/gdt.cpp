

#include <gdt.h>

using namespace cpp_os::common;
using namespace cpp_os;

GDT::GDT()
        : _null_segment_selector(0, 0, 0),
          _unused_segment_selector(0, 0, 0),
          _code_segment_selector(0, 64 * 1024 * 1024, 0x9A),
          _data_segment_selector(0, 64 * 1024 * 1024, 0x92) {
    uint32_t i[2];
    i[1] = (uint32_t) this; //size of table itself
    i[0] = sizeof(GDT) << 16;
    //lgdt - load global descriptor table
    asm volatile(
            "lgdt (%0)\n\t"
            "mov %1, %%ss\n\t"
            "mov %1, %%ds\n\t"
            "mov %1, %%es\n\t"
            "mov %1, %%fs\n\t"
            "mov %1, %%gs\n\t"
            "push %k2\n\t"
            "push $1f\n\t"
            "ljmp *(%%esp)\n"
            "1:\n\t"
            "add $8, %%esp\n\t"
            :
            : "r" (((uint8_t *) i)+2),
            "r" (DataSegmentSelector()),
            "r" (CodeSegmentSelector())
            : "memory");
}

GDT::~GDT() {
}

uint16_t GDT::DataSegmentSelector() {
    return (uint8_t *) &_data_segment_selector - (uint8_t *) this;
}

uint16_t GDT::CodeSegmentSelector() {
    return (uint8_t *) &_code_segment_selector - (uint8_t *) this;
}

GDT::SD::SD(uint32_t base, uint32_t limit, uint8_t type) {
    uint8_t *target = (uint8_t *) this;

    if (limit <= 65536) {
        target[6] = 0x40;//16 bit entry
    } else {

        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;

        target[6] = 0xC0;
    }

    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Type
    target[5] = type;
}

uint32_t GDT::SD::Base() {
    uint8_t *target = (uint8_t *) this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GDT::SD::Limit() {
    uint8_t *target = (uint8_t *) this;

    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xFFF;

    return result;
}

