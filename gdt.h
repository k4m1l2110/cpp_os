//
// Created by kamil on 30.10.23.
//

#ifndef _GDT_H
#define _GDT_H

#include "types.h"

//Global Descriptor Table
class GDT {
public:
    //Segment descriptor
    class SD {
    private:
        uint16_t _limit_lo;      //limit low bytes
        uint16_t _base_lo;       //ptr low bytes
        uint8_t _base_hi;        //ptr extension
        uint8_t _type;           //access bytes
        uint8_t _flags_limit_hi; //base_vhi
        uint8_t _base_vhi;       //another byte for ptr
    public:
        SD(uint32_t base, uint32_t limit, uint8_t flags);
        uint32_t Base();
        uint32_t Limit();
    }__attribute__((packed));// Making sure the class will be addressed like in code

    SD _null_segment_selector;
    SD _unused_segment_selector;
    SD _code_segment_selector;
    SD _data_segment_selector;

public:
    GDT();
    ~GDT();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};


#endif
