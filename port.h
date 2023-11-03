//
// Created by kamil on 01.11.23.
//

#ifndef _PORT_H
#define _PORT_H

#include "types.h"

class Port {
protected:
    uint16_t _port_number;
    Port(uint16_t _port_nr):_port_number(_port_nr) { ; }
    ~Port() { ; }
};

class Port8_t:public Port{
public:
    Port8_t(uint16_t _port_nr): Port(_port_nr) { ; }
    ~Port8_t(){;}
    void Write(uint8_t data);
    uint8_t Read();
};

class Port8_t_slow:public Port8_t{
public:
    Port8_t_slow(uint16_t _port_nr): Port8_t(_port_nr) { ; }
    ~Port8_t_slow(){;}
    void Write(uint8_t data);
};

class Port16_t:public Port{
public:
    Port16_t(uint16_t _port_nr): Port(_port_nr) { ; }
    ~Port16_t(){;}
    void Write(uint16_t data);
    uint16_t Read();
};

class Port32_t:public Port{
public:
    Port32_t(uint16_t _port_nr): Port(_port_nr) { ; }
    ~Port32_t(){;}
    void Write(uint32_t data);
    uint32_t Read();
};

#endif
