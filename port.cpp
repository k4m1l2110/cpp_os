//
// Created by kamil on 01.11.23.
//

#include "port.h"

void Port8_t::Write(uint8_t data){
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (_port_number));
}

uint8_t Port8_t::Read(){
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port_number));
    return result;
}

void Port8_t_slow::Write(uint8_t data){
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (_port_number));
}

void Port16_t::Write(uint16_t data){
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (_port_number));
}

uint16_t Port16_t::Read(){
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port_number));
    return result;
}

void Port32_t::Write(uint32_t data){
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (_port_number));
}

uint32_t Port32_t::Read(){
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port_number));
    return result;
}