#ifndef __CUSTOMOS_PCI_H
#define __CUSTOMOS_PCI_H

#include <hardwareCommunication/port.h>
#include <hardwareCommunication/interrupts.h>
#include <general/types.h>
#include <drivers/driver.h>
namespace customOS{
    namespace hardwareCommunication{

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };
        
        
        class BaseAddressRegister
        {
        public:
            bool prefetchable;
            customOS::general::uint8_t* address;
            customOS::general::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PCIDeviceDescriptor{
            public:
            customOS::general::uint32_t Port;
            customOS::general::uint32_t Interrupt;

            customOS::general::uint16_t Bus;
            customOS::general::uint16_t Device;
            customOS::general::uint16_t Function;

            customOS::general::uint16_t Vendor_id;
            customOS::general::uint16_t Device_id;

            customOS::general::uint8_t Class_id;
            customOS::general::uint8_t Subclass_id;
            customOS::general::uint8_t Interface_id;

            customOS::general::uint8_t Revision;

            PCIDeviceDescriptor();
            ~PCIDeviceDescriptor();
        };

        class PCI{
            private:
            Port32Bit commandPort;
            Port32Bit dataPort;

            public:
            PCI();
            ~PCI();

            customOS::general::int32_t Read(customOS::general::int16_t Bus, customOS::general::int16_t Device, customOS::general::int16_t Function, customOS::general::int32_t offsetRegister);
            void Write(customOS::general::int16_t Bus, customOS::general::int16_t Device, customOS::general::int16_t Function, customOS::general::int32_t offsetRegister, customOS::general::int32_t msg);
            bool HasFunctions(customOS::general::int16_t Bus, customOS::general::int16_t Device);

            void SelectDrivers(customOS::drivers::DriverManager* driverManager, customOS::hardwareCommunication::InterruptManager* manager);
            PCIDeviceDescriptor GetDeviceDescriptor(customOS::general::int16_t Bus, customOS::general::int16_t Device, customOS::general::int16_t Function);

            customOS::drivers::Driver* GetDriver(PCIDeviceDescriptor dev, customOS::hardwareCommunication::InterruptManager* interrupts);
            BaseAddressRegister GetBaseAddressRegister(customOS::general::uint16_t bus, customOS::general::uint16_t device, customOS::general::uint16_t function, customOS::general::uint16_t bar);

        };


    }
}

#endif
