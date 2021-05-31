#ifndef __CUSTOMOS_INTERRUPTMANAGER_H
#define __CUSTOMOS_INTERRUPTMANAGER_H

#include <general/types.h>
#include <hardwareCommunication/port.h>
#include <gdt.h>

namespace customOS{
    namespace hardwareCommunication{

        class InterruptManager;

        class InterruptHandler{
            protected:
            customOS::general::uint8_t interruptNumber;
            InterruptManager* interruptManager;
            InterruptHandler(customOS::general::uint8_t interNumber, InterruptManager* interruptManager);
            ~InterruptHandler();

            public:
            virtual customOS::general::uint32_t HandleInterrupt(customOS::general::uint32_t esp);
        };


        class InterruptManager{

            friend class InterruptHandler;
            protected:

            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            struct GateDescriptor
            {
                customOS::general::uint16_t handlerAddressLowBIts;
                customOS::general::uint16_t gdt_codeSegmentSelector;
                customOS::general::uint8_t reserved;
                customOS::general::uint8_t access;
                customOS::general::uint16_t handlerAddressHighBIts;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer{
                customOS::general::uint16_t size;
                customOS::general::uint32_t base;
            }__attribute__((packed));

            customOS::general::uint16_t hardwareInterruptOffset;

            static void SetInterruptDescriptorTable(
                customOS::general::uint8_t interruptnumber,
                customOS::general::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                customOS::general::uint8_t DescriptorPrivilageLevel,
                customOS::general::uint8_t DescriptorType);


            Port8BitSlow picMasterCommand;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveData;


            static customOS::general::uint32_t HandleInterrupt(customOS::general::uint8_t interruptNumber, customOS::general::uint32_t esp);
            customOS::general::uint32_t DoHandleInterrupt(customOS::general::uint8_t interruptNumber, customOS::general::uint32_t esp);

            static void IgnoreInterrupt();
            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();


            public:
            InterruptManager(GlobalDescriptorTable* gdt);
            ~InterruptManager();
            void Activate();
            void Deactivate();

        };
    }
}

#endif
