//
// Created by kamil on 05.11.23.
//

#include "mouse.h"

void Print(char *str);

MouseDriver::MouseDriver(InterruptManager *IM)
: InterruptHandler(0x2C,IM), data_port(0x60), command_port(0x64){

    offset = 0;
    buttons = 0;
    //Wait if key is being pressed and erase previous key strikes
    uint16_t *VideoMemory = (uint16_t *) 0xb8000;
    const uint16_t x = 40, y = 12;

    VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));

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

MouseDriver::~MouseDriver() {

}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
    uint8_t status = command_port.Read();
    if(!(status & 0x20))
        return esp;

    static int8_t x = 40, y = 12;

    buffer[offset] = data_port.Read();
    offset = (offset + 1) % 3;

    if(offset == 0){

        static uint16_t *VideoMemory = (uint16_t *) 0xb8000;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));

        x+= buffer[1];
        if(x < 0) x = 0;
        y-= buffer[2];
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));
    }

    for(uint8_t i = 0; i < 3; i++){
        if((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))){
            if(buttons & (0x1 << i))
                Print("Button released\n");
            else
                Print("Button pressed\n");
        }
    }

    buttons = buffer[0];

    return esp;
}