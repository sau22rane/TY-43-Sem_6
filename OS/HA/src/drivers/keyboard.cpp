#include <drivers/keyboard.h>
using namespace customOS::drivers;
using namespace customOS::hardwareCommunication;
using namespace customOS::general;

void printf(char *);

KeyboardDriver::KeyboardDriver(InterruptManager* interruptManager, KeyboardEventHandler* handler) : 
InterruptHandler(0x21, interruptManager), 
dataPort(0x60),
commandPort(0x64){
    this->handler = handler;
}

void KeyboardDriver::Activate(){
     while(commandPort.read() & 0x1)    // wait for key to release
        dataPort.read();
    commandPort.write(0xAE);            // activates interrupts
    commandPort.write(0x20);            // current state
    uint8_t status = (dataPort.read() | 1) & ~0x10;
    commandPort.write(0x60);            // change the current state

    dataPort.write(status);
    dataPort.write(0xF4);
}


KeyboardDriver::~KeyboardDriver(){

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
    static int Caps = 0, Shift = 0;
    uint8_t key = dataPort.read();

    // char* msg = "Keyboard 0x00 ";
    // char* hex = "0123456789ABCDEF";
    // msg[11] = hex[(key>>4) & 0xF];
    // msg[12] = hex[(key) & 0xF];
    // printf(msg);

    if(handler==0){
        printf("Error...with handler");
        return esp;
    }

    if(key<0x80){

        switch(key){
            case 0x45: break;
            case 0x02: 
                this->handler->OnKeyDown("1"); 
                break;
            case 0x03: 
                this->handler->OnKeyDown("2"); 
                break;
            case 0x04: 
                this->handler->OnKeyDown("3"); 
                break;
            case 0x05: 
                this->handler->OnKeyDown("4"); 
                break;
            case 0x06: 
                this->handler->OnKeyDown("5"); 
                break;
            case 0x07: 
                this->handler->OnKeyDown("6"); 
                break;
            case 0x08: 
                this->handler->OnKeyDown("7"); 
                break;
            case 0x09: 
                this->handler->OnKeyDown("8"); 
                break;
            case 0x0A: 
                this->handler->OnKeyDown("9"); 
                break;
            case 0x0B: 
                this->handler->OnKeyDown("0"); 
                break;

            case 0x10: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("q");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("Q"); 
                break;
            case 0x11: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("w");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("W"); 
                break;
            case 0x12: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("e");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("E"); 
                break;
            case 0x13: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("r");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("R"); 
                break;
            case 0x14: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("t");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("T"); 
                break;
            case 0x15: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("y");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("Y"); 
                break;
            case 0x16: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("u");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("U"); 
                break;
            case 0x17: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("i");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("I"); 
                break;
            case 0x18: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("o");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("O"); 
                break;
            case 0x19: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("p");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("P"); 
                break;

            case 0x1E: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("a");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("A"); 
                break;
            case 0x1F: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("s");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("S"); 
                break;
            case 0x20: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("d");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("D"); 
                break;
            case 0x21: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("f");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("F"); 
                break;
            case 0x22: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("g");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("G"); 
                break;
            case 0x23: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("h");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("H"); 
                break;
            case 0x24: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("j");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("J"); 
                break;
            case 0x25: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("k");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("K"); 
                break;
            case 0x26: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("l");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("L"); 
                break;

            case 0x2C: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("z");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("Z"); 
                break;
            case 0x2D: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("x");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("X"); 
                break;
            case 0x2E: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("c");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("C"); 
                break;
            case 0x2F: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("v");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("V"); 
                break;
            case 0x30: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("b");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("B"); 
                break;
            case 0x31: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("n");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("N"); 
                break;
            case 0x32: 
                if((Caps==1 && Shift == 1) || (Caps==0 && Shift == 0))
                    this->handler->OnKeyDown("m");
                else if(Caps==1 || Shift == 1)
                    this->handler->OnKeyDown("M"); 
                break;
            case 0x33: 
                this->handler->OnKeyDown(","); 
                break;
            case 0x34: 
                this->handler->OnKeyDown("."); 
                break;
            case 0x35: 
                this->handler->OnKeyDown("-"); 
                break;

            case 0x1C: 
                this->handler->OnKeyDown("\n");
                 break;
            case 0x39: 
                this->handler->OnKeyDown(" "); 
                break;
            case 0x0F: 
                this->handler->OnKeyDown("\t");
                 break;
            case 0x0E: 
                this->handler->OnKeyDown("\b");
                 break;
            
            default:
                // char* msg = "Keyboard 0x00 ";
                // char* hex = "0123456789ABCDEF";
                // msg[11] = hex[(key>>4) & 0xF];
                // msg[12] = hex[(key) & 0xF];
                // printf(msg);

                break;

        }
    }

    if(key ==  0x2A){ 
        Shift = 1;
    }
    if(key == 0xAA){
        Shift = 0;
    }
    if(key == 0xBA){
        if(Caps==0)
            Caps = 1;
        else
            Caps = 0;
    }

    return esp;

}



KeyboardEventHandler::KeyboardEventHandler(){

}

void KeyboardEventHandler::OnKeyDown(char* key){
    printf(key);
    // printf("Here");

}


void KeyboardEventHandler::OnKeyUp(char* key){
    
}