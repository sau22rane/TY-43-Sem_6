#ifndef __CUSTOMOS_KEYBOARD_H
#define __CUSTOMOS_KEYBOARD_H

#include <general/types.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>

namespace customOS{
    namespace drivers{

        class KeyboardEventHandler{
            public:
            KeyboardEventHandler();
            ~KeyboardEventHandler();
            virtual void OnKeyDown(char* key);
            virtual void OnKeyUp(char* key);
        };



        class KeyboardDriver: public customOS::hardwareCommunication::InterruptHandler, public customOS::drivers::Driver{
            customOS::hardwareCommunication::Port8Bit dataPort;
            customOS::hardwareCommunication::Port8Bit commandPort;
            KeyboardEventHandler* handler;

            public:
            KeyboardDriver(customOS::hardwareCommunication::InterruptManager* interruptManager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual customOS::general::uint32_t HandleInterrupt(customOS::general::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif