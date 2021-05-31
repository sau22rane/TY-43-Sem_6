#ifndef __CUSTOMOS_GDT_H
#define __CUSTOMOS_GDT_H

#include <general/types.h>
namespace customOS{
    class GlobalDescriptorTable{
        public:
        class SegmentDescriptor{
            private:
            customOS::general::uint16_t limit_lb;
            customOS::general::uint16_t base_lb;
            customOS::general::uint8_t base_hb1;
            customOS::general::uint8_t flag_lb;
            customOS::general::uint8_t flag_hb_limit_hb;
            customOS::general::uint8_t base_hb2;

            public:
            SegmentDescriptor(customOS::general::uint32_t base, customOS::general::uint32_t limit, customOS::general::uint8_t flags);
            customOS::general::uint32_t Base();
            customOS::general::uint32_t Limit();
        } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;      // (0,64*1024*1024,0x9A)    flags => 1001 1010
        SegmentDescriptor dataSegmentSelector;      // (0,64*1024*1024,0x92)    flags => 1001 0010

        public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();
        customOS::general::uint16_t CodeSegmentSelector();
        customOS::general::uint16_t DataSegmentSelector();
    };
}
#endif
