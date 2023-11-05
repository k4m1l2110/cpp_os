//
// Created by kamil on 05.11.23.
//

#include "keyboard.h"

void Print(char *str);

KeyboardDriver::KeyboardDriver(InterruptManager *IM)
: InterruptHandler(0x21,IM), data_port(0x60), command_port(0x64){
    //Wait if key is being pressed and erase previous key strikes
    while(command_port.Read() & 0x1)
        data_port.Read();
    //Activate communication between keyboard and intterrupt manager
    command_port.Write(0xAE);
    //Get current state
    command_port.Write(0x20);
    //Read rightmost bit as new state
    uint8_t status = (data_port.Read() | 1) & ~0x10;
    //0x60 command to tell pic to change current state
    command_port.Write(0x60);
    data_port.Write(status);

    //Activate the keyboard input
    data_port.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver() {

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
    uint8_t key = data_port.Read();
    //Above 0x80 are key relase codes
    if(key < 0x80) {
        char key_char;
        switch(key)
        {
            case 0x02: key_char='1'; break;
            case 0x03: key_char='2'; break;
            case 0x04: key_char='3'; break;
            case 0x05: key_char='4'; break;
            case 0x06: key_char='5'; break;
            case 0x07: key_char='6'; break;
            case 0x08: key_char='7'; break;
            case 0x09: key_char='8'; break;
            case 0x0A: key_char='9'; break;
            case 0x0B: key_char='0'; break;

            case 0x10: key_char='q'; break;
            case 0x11: key_char='w'; break;
            case 0x12: key_char='e'; break;
            case 0x13: key_char='r'; break;
            case 0x14: key_char='t'; break;
            case 0x15: key_char='z'; break;
            case 0x16: key_char='u'; break;
            case 0x17: key_char='i'; break;
            case 0x18: key_char='o'; break;
            case 0x19: key_char='p'; break;

            case 0x1E: key_char='a'; break;
            case 0x1F: key_char='s'; break;
            case 0x20: key_char='d'; break;
            case 0x21: key_char='f'; break;
            case 0x22: key_char='g'; break;
            case 0x23: key_char='h'; break;
            case 0x24: key_char='j'; break;
            case 0x25: key_char='k'; break;
            case 0x26: key_char='l'; break;

            case 0x2C: key_char='y'; break;
            case 0x2D: key_char='x'; break;
            case 0x2E: key_char='c'; break;
            case 0x2F: key_char='v'; break;
            case 0x30: key_char='b'; break;
            case 0x31: key_char='n'; break;
            case 0x32: key_char='m'; break;
            case 0x33: key_char=','; break;
            case 0x34: key_char='.'; break;
            case 0x35: key_char='-'; break;

            case 0x1C: key_char='\n'; break;
            case 0x39: key_char=' '; break;
            default:
                break;
        }
        char *msg="\0";
        msg[0]=key_char;
        //char *msg = "KEYBOARD 0x00 ";
        //char *hex = "0123456789ABCDEF";

        //msg[11] = hex[(key >> 4) & 0x0F];
        //msg[12] = hex[key & 0x0F];
        Print(msg);
    }
    return esp;
}