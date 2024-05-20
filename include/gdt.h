//
// Created by kamil on 30.10.23.
//

#ifndef __CPPOS__GDT_H
#define __CPPOS__GDT_H

#include <common/types.h>

namespace cpp_os {

    class GDT {
    public:
        class SD {
        private:
            cpp_os::common::uint16_t _limit_lo;      //limit low bytes
            cpp_os::common::uint16_t _base_lo;       //ptr low bytes
            cpp_os::common::uint8_t _base_hi;        //ptr extension
            cpp_os::common::uint8_t _type;           //access bytes
            cpp_os::common::uint8_t _flags_limit_hi; //base_vhi
            cpp_os::common::uint8_t _base_vhi;       //another byte for ptr

        public:
            //Segment descriptor
            SD(cpp_os::common::uint32_t base, cpp_os::common::uint32_t limit, cpp_os::common::uint8_t type);

            cpp_os::common::uint32_t Base();

            cpp_os::common::uint32_t Limit();
        } __attribute__((packed));

    private:
        SD _null_segment_selector;
        SD _unused_segment_selector;
        SD _code_segment_selector;
        SD _data_segment_selector;

    public:
        //Global Descriptor Table
        GDT();

        ~GDT();

        cpp_os::common::uint16_t CodeSegmentSelector();

        cpp_os::common::uint16_t DataSegmentSelector();
    };
}

#endif
