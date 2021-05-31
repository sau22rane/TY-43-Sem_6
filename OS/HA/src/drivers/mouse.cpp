#include <drivers/mouse.h>
using namespace customOS::drivers;
using namespace customOS::hardwareCommunication;
using namespace customOS::general;

void printf(char *);

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),
dataport(0x60),
commandport(0x64){
    this->handler = handler;
}

void MouseDriver::Activate(){
    offset = 0;
    buttons = 0;

    commandport.write(0xA8);
    commandport.write(0x20); // command 0x60 = read controller command byte
    uint8_t status = dataport.read() | 2;
    commandport.write(0x60); // command 0x60 = set controller command byte
    dataport.write(status);

    commandport.write(0xD4);
    dataport.write(0xF4);
    dataport.read();
}

MouseDriver::~MouseDriver(){
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
    uint8_t status = commandport.read();
    if (!(status & 0x20) || handler==0)
        return esp;

    buffer[offset] = dataport.read();
    offset = (offset + 1) % 3;

    if(offset == 0)    {
        if(buffer[1] != 0 || buffer[2] != 0){
            this->handler->OnMouseMove(buffer[1], buffer[2]);
        }
    if(buffer[1] != 0 || buffer[2] != 0){
            handler->OnMouseMove(buffer[1], -buffer[2]);
        }

        for(uint8_t i = 0; i < 3; i++){
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i))){
                if(buttons & (0x1<<i))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
        }
        buttons = buffer[0];
       
    }
    return esp;
}



MouseEventHandler::MouseEventHandler(){

}

void MouseEventHandler::OnMouseDown(uint8_t button){
    // char* msg = "Down 0x00 ";
    // char* hex = "0123456789ABCDEF";
    // msg[11] = hex[(button>>4) & 0xF];
    // msg[12] = hex[(button) & 0xF];
    // printf(msg);
}

void MouseEventHandler::OnMouseUp(uint8_t button){
    //  char* msg = "Up 0x00 ";
    // char* hex = "0123456789ABCDEF";
    // msg[11] = hex[(button>>4) & 0xF];
    // msg[12] = hex[(button) & 0xF];
    // printf(msg);
}

void MouseEventHandler::OnMouseMove(int8_t i, int8_t j){

}
void MouseEventHandler::onActivate(){

}