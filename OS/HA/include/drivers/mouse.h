#ifndef __CUSTOMOS_MOUSE_H
#define __CUSTOMOS_MOUSE_H

#include <general/types.h>
#include <hardwareCommunication/port.h>
#include <hardwareCommunication/interrupts.h>
#include <drivers/driver.h>

namespace customOS{
    namespace drivers{

        class MouseEventHandler{
            public:
            MouseEventHandler();
            ~MouseEventHandler();
            virtual void onActivate();
            virtual void OnMouseDown(customOS::general::uint8_t button);
            virtual void OnMouseUp(customOS::general::uint8_t button);
            virtual void OnMouseMove(customOS::general::int8_t i, customOS::general::int8_t j);
        };

        class MouseDriver : public customOS::hardwareCommunication::InterruptHandler, public customOS::drivers::Driver{
            customOS::hardwareCommunication::Port8Bit dataport;
            customOS::hardwareCommunication::Port8Bit commandport;
            customOS::general::uint8_t buffer[3];
            customOS::general::uint8_t offset;
            customOS::general::uint8_t buttons;
            MouseEventHandler* handler;

            customOS::general::int8_t x, y;
        public:
            MouseDriver(customOS::hardwareCommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual customOS::general::uint32_t HandleInterrupt(customOS::general::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif