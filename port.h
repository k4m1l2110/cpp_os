//
// Created by kamil on 01.11.23.
//

#ifndef _PORT_H
#define _PORT_H

#include "types.h"

class port
{
protected:
    port(uint16_t portnumber);
    // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
    ~port();
    uint16_t portnumber;
};


class port8_t : public port
{
public:
    port8_t(uint16_t portnumber);
    ~port8_t();

    virtual uint8_t Read();
    virtual void Write(uint8_t data);

protected:
    static inline uint8_t Read8(uint16_t _port)
    {
        uint8_t result;
        __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
        return result;
    }

    static inline void Write8(uint16_t _port, uint8_t _data)
    {
        __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
    }
};



class port8_t_slow : public port8_t
{
public:
    port8_t_slow(uint16_t portnumber);
    ~port8_t_slow();

    virtual void Write(uint8_t data);
protected:
    static inline void Write8Slow(uint16_t _port, uint8_t _data)
    {
        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
    }

};



class port16_t : public port
{
public:
    port16_t(uint16_t portnumber);
    ~port16_t();

    virtual uint16_t Read();
    virtual void Write(uint16_t data);

protected:
    static inline uint16_t Read16(uint16_t _port)
    {
        uint16_t result;
        __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
        return result;
    }

    static inline void Write16(uint16_t _port, uint16_t _data)
    {
        __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
    }
};



class port32_t : public port
{
public:
    port32_t(uint16_t portnumber);
    ~port32_t();

    virtual uint32_t Read();
    virtual void Write(uint32_t data);

protected:
    static inline uint32_t Read32(uint16_t _port)
    {
        uint32_t result;
        __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
        return result;
    }

    static inline void Write32(uint16_t _port, uint32_t _data)
    {
        __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
    }
};

#endif
