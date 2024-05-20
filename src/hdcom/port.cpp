//
// Created by kamil on 01.11.23.
//

#include <hdcom/port.h>

using namespace cpp_os::hdcom;
using namespace cpp_os::common;

port::port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

port::~port()
{
}


port8_t::port8_t(uint16_t portnumber)
        : port(portnumber)
{
}

port8_t::~port8_t()
{
}

void port8_t::Write(uint8_t data)
{
    Write8(portnumber, data);
}

uint8_t port8_t::Read()
{
    return Read8(portnumber);
}





port8_t_slow::port8_t_slow(uint16_t portnumber)
        : port8_t(portnumber)
{
}

port8_t_slow::~port8_t_slow()
{
}

void port8_t_slow::Write(uint8_t data)
{
    Write8Slow(portnumber, data);
}





port16_t::port16_t(uint16_t portnumber)
        : port(portnumber)
{
}

port16_t::~port16_t()
{
}

void port16_t::Write(uint16_t data)
{
    Write16(portnumber, data);
}

uint16_t port16_t::Read()
{
    return Read16(portnumber);
}





port32_t::port32_t(uint16_t portnumber)
        : port(portnumber)
{
}

port32_t::~port32_t()
{
}

void port32_t::Write(uint32_t data)
{
    Write32(portnumber, data);
}

uint32_t port32_t::Read()
{
    return Read32(portnumber);
}