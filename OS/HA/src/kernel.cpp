#include <general/types.h>
#include <gdt.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/mouse.h>
#include <drivers/keyboard.h>

using namespace customOS::drivers;
using namespace customOS::hardwareCommunication;
using namespace customOS::general;
using namespace customOS;

void printf(char*);

class PrintfKeyboard : public KeyboardEventHandler{
    virtual void OnKeyDown(char* key){
        printf(key);
    }
};


void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class MouseOnConsole: public MouseEventHandler{
    private:
    int8_t x, y;
    public:
    MouseOnConsole(){
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    x = 40;
    y = 12;
    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                        | (VideoMemory[80*y+x] & 0xF000) >> 4
                        | (VideoMemory[80*y+x] & 0x00FF);

    }
    void OnMouseMove(int8_t i, int8_t j){
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                | (VideoMemory[80*y+x] & 0xF000) >> 4
                                | (VideoMemory[80*y+x] & 0x00FF);

            x += i;
            if(x >= 80) 
                x = 79;
            if(x < 0) 
                x = 0;
            
            y += j;

            if(y >= 25) 
                y = 24;
            if(y < 0) 
                y = 0;

            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                | (VideoMemory[80*y+x] & 0xF000) >> 4
                                | (VideoMemory[80*y+x] & 0x00FF);
    }
};



void printf(int8_t *str){
    static uint16_t* VideoMem = (uint16_t*)0xb8000;
    static uint16_t x = 0,y = 0;
    for(int32_t i = 0; str[i]!='\0';i++){

        switch(str[i]){
            case '\b':
            if(x==0 && y!=0){
                y--;
                x = 79;
            }
            else if(x!=0){
            x--;
            }
            VideoMem[80*y + x+1] = (VideoMem[80*y + x+1]&0xFF00) | ' ';
            break;

            case '\t':
            x += 4;
            break;

            case '\n':
            x = 0;
            y++;
            break;

            default:
            VideoMem[80*y + x] = (VideoMem[80*y + x]&0xFF00) | str[i];
            x++;
            break;
        }
        if(x>=80){
            y++;
            x=0;
        }
        if(y>=25){
            for(int i = 0; i<80;i++)
                for(int j = 0; j<25; j++)
                    VideoMem[80*j + i] = (VideoMem[80*y + x]&0xFF00) | ' ';

            x = 0;
            y = 0;
        }
        VideoMem[80*y + x] = (VideoMem[80*y + x]&0xFF00) | '|';
    }
}

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors; 

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; *i!=end_ctors;i++){
        (*i)();
    }
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magic_number){
    int8_t *msg = (int8_t*) "Welcome Saurabh Rane...!!!";
    printf(msg);
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    DriverManager driverManager;

    PrintfKeyboard keyboardHandler;
    KeyboardDriver keyboard(&interrupts, &keyboardHandler);
    driverManager.AddDriver(&keyboard);

    MouseOnConsole mouseHandler;
    MouseDriver mouse(&interrupts, &mouseHandler);
    driverManager.AddDriver(&mouse);

    PCI pciController;
    pciController.SelectDrivers(&driverManager, &interrupts);

    driverManager.ActivateAll();

    interrupts.Activate();


    while(1);
}
