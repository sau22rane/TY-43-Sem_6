#ifndef __CUSTOMOS_PORT_H
#define __CUSTOMOS_PORT_H

#include <general/types.h>

namespace customOS{
    namespace hardwareCommunication{

        class Port{
            protected:
            customOS::general::uint16_t portnumber;
            Port(customOS::general::uint16_t portnumber);
            ~Port();
        };

        class Port8Bit : public Port{
            public:
            Port8Bit(customOS::general::int16_t portnumber);
            ~Port8Bit();
            virtual void write(customOS::general::uint8_t data);
            virtual customOS::general::uint8_t read();
        };

        class Port16Bit : public Port{
            public:
            Port16Bit(customOS::general::int16_t portnumber);
            ~Port16Bit();
            virtual void write(customOS::general::uint16_t data);
            virtual customOS::general::uint16_t read();
        };

        class Port32Bit : public Port{
            public:
            Port32Bit(customOS::general::int16_t portnumber);
            ~Port32Bit();
            virtual void write(customOS::general::uint32_t data);
            virtual customOS::general::uint32_t read();
        };

        class Port8BitSlow : public Port8Bit{
            public:
            Port8BitSlow(customOS::general::int16_t portnumber);
            ~Port8BitSlow();
            virtual void write(customOS::general::uint8_t data);
        };
    }
}

#endif
