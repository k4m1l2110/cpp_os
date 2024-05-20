//
// Created by kamil on 05.11.23.
//

#include <drivers/io/mouse.h>

using namespace cpp_os::hdcom;
using namespace cpp_os::common;
using namespace cpp_os::drivers::io;

void Print(char *str);

MouseEventHandler::MouseEventHandler() {
    uint16_t *VideoMemory = (uint16_t *) 0xb8000;
    x = 40;
    y = 12;

    VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));
}

MouseEventHandler::~MouseEventHandler(){;}

void MouseEventHandler::OnKeyDown(uint8_t button){
    if(button == 1)
        Print("Left button pressed\n");
    else if(button == 2)
        Print("Right button pressed\n");
    else if(button == 3)
        Print("Middle button pressed\n");
}

void MouseEventHandler::OnKeyUp(uint8_t button){
    if(button == 1)
        Print("Left button released\n");
    else if(button == 2)
        Print("Right button released\n");
    else if(button == 3)
        Print("Middle button released\n");
}

void MouseEventHandler::OnMouseMove(int dx, int dy){
    static uint16_t *VideoMemory = (uint16_t *) 0xb8000;

    VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                        | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                        | ((VideoMemory[80*y+x] & 0x00FF));

    x+= dx;
    if(x < 0) x = 0;
    y+= dy;
    if(y < 0) y = 0;

    VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                        | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                        | ((VideoMemory[80*y+x] & 0x00FF));
}

MouseDriver::MouseDriver(InterruptManager *IM, MouseEventHandler *event_handler)
: InterruptHandler(0x2C,IM), data_port(0x60), command_port(0x64), event_handler(event_handler)
{
}

MouseDriver::~MouseDriver() {

}

void MouseDriver::Activate() {
    offset = 0;
    buttons = 0;
    //Wait if key is being pressed and erase previous key strikes


    command_port.Write(0xAE);
    //Get current state
    command_port.Write(0x20);
    //Read rightmost bit as new state
    uint8_t status = (data_port.Read() | 2);
    //0x60 command to tell pic to change current state
    command_port.Write(0x60);
    data_port.Write(status);

    command_port.Write(0xD4);
    data_port.Write(0xF4);
    data_port.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
    uint8_t status = command_port.Read();

    if(!(status & 0x20))
        return esp;

    buffer[offset] = data_port.Read();

    if(!event_handler)
        return esp;

    offset = (offset + 1) % 3;

    if(offset == 0){
        if(buffer[0] != 0 || buffer[1] !=0){

        event_handler->OnMouseMove(buffer[1], -buffer[2]);

        }

        for(uint8_t i = 0; i < 3; i++){
            if((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))){
                if(buttons & (0x1 << i))
                    event_handler->OnKeyUp(i+1);
                else
                    event_handler->OnKeyDown(i+1);
            }
        }
        buttons = buffer[0];
    }


    return esp;
}