//
// Created by kamil on 05.11.23.
//

#include <drivers/io/keyboard.h>

void Print(char *str);

void PrintHex(cpp_os::common::uint8_t key);

void cpp_os::drivers::io::PrintKeyboardEventHandler::OnKeyDown(char c) {
    char *res = " ";
    res[0] = c;
    Print(res);
}

cpp_os::drivers::io::KeyboardDriver::KeyboardDriver(cpp_os::hdcom::InterruptManager *IM, cpp_os::drivers::io::EventHandler *event_handler)
: cpp_os::hdcom::InterruptHandler(0x21,IM), data_port(0x60), command_port(0x64), event_handler(event_handler)
{

}

cpp_os::drivers::io::KeyboardDriver::~KeyboardDriver() {

}

void cpp_os::drivers::io::KeyboardDriver::Activate() {
    //Wait if key is being pressed and erase previous key strikes
    while(command_port.Read() & 0x1)
        data_port.Read();
    //Activate communication between keyboard and intterrupt manager
    command_port.Write(0xAE);
    //Get current state
    command_port.Write(0x20);
    //Read rightmost bit as new state
    cpp_os::common::uint8_t status = (data_port.Read() | 1) & ~0x10;
    //0x60 command to tell pic to change current state
    command_port.Write(0x60);
    data_port.Write(status);

    //Activate the keyboard input
    data_port.Write(0xF4);
}


cpp_os::common::uint32_t cpp_os::drivers::io::KeyboardDriver::HandleInterrupt(cpp_os::common::uint32_t esp){
    cpp_os::common::uint8_t key = data_port.Read();
    //Above 0x80 are key release codes

    if(key > 0x80)
        return esp;

    if(key < 0x80) {
        switch(key)
        {
            case 0x02: event_handler->OnKeyDown('1'); break;
            case 0x03: event_handler->OnKeyDown('2'); break;
            case 0x04: event_handler->OnKeyDown('3'); break;
            case 0x05: event_handler->OnKeyDown('4'); break;
            case 0x06: event_handler->OnKeyDown('5'); break;
            case 0x07: event_handler->OnKeyDown('6'); break;
            case 0x08: event_handler->OnKeyDown('7'); break;
            case 0x09: event_handler->OnKeyDown('8'); break;
            case 0x0A: event_handler->OnKeyDown('9'); break;
            case 0x0B: event_handler->OnKeyDown('0'); break;

            case 0x10: event_handler->OnKeyDown('q'); break;
            case 0x11: event_handler->OnKeyDown('w'); break;
            case 0x12: event_handler->OnKeyDown('e'); break;
            case 0x13: event_handler->OnKeyDown('r'); break;
            case 0x14: event_handler->OnKeyDown('t'); break;
            case 0x15: event_handler->OnKeyDown('z'); break;
            case 0x16: event_handler->OnKeyDown('u'); break;
            case 0x17: event_handler->OnKeyDown('i'); break;
            case 0x18: event_handler->OnKeyDown('o'); break;
            case 0x19: event_handler->OnKeyDown('p'); break;

            case 0x1E: event_handler->OnKeyDown('a'); break;
            case 0x1F: event_handler->OnKeyDown('s'); break;
            case 0x20: event_handler->OnKeyDown('d'); break;
            case 0x21: event_handler->OnKeyDown('f'); break;
            case 0x22: event_handler->OnKeyDown('g'); break;
            case 0x23: event_handler->OnKeyDown('h'); break;
            case 0x24: event_handler->OnKeyDown('j'); break;
            case 0x25: event_handler->OnKeyDown('k'); break;
            case 0x26: event_handler->OnKeyDown('l'); break;

            case 0x2C: event_handler->OnKeyDown('y'); break;
            case 0x2D: event_handler->OnKeyDown('x'); break;
            case 0x2E: event_handler->OnKeyDown('c'); break;
            case 0x2F: event_handler->OnKeyDown('v'); break;
            case 0x30: event_handler->OnKeyDown('b'); break;
            case 0x31: event_handler->OnKeyDown('n'); break;
            case 0x32: event_handler->OnKeyDown('m'); break;
            case 0x33: event_handler->OnKeyDown(','); break;
            case 0x34: event_handler->OnKeyDown('.'); break;
            case 0x35: event_handler->OnKeyDown('-'); break;

            case 0x1C: event_handler->OnKeyDown('\n'); break;
            case 0x39: event_handler->OnKeyDown(' '); break;
            default:
                Print("KEYBOARD_0x");
                PrintHex(key);
                break;
        }
    }
    return esp;
}