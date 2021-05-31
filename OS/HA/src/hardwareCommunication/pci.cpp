#include <hardwareCommunication/pci.h>
using namespace customOS::hardwareCommunication;
using namespace customOS::general;
using namespace customOS::drivers;

void printf(char*);
void printfHex(uint8_t);

PCI::PCI()
: dataPort(0xCFC),
  commandPort(0xCF8){

}

PCI::~PCI(){

}

int32_t PCI::Read(int16_t Bus, int16_t Device, int16_t Function, int32_t offsetRegister){
    uint32_t id =   0x1<<31
                    | ((Bus & 0xFF) << 16)
                    | ((Device & 0x1F) << 11)
                    | ((Function & 0x07) << 8)
                    | ((offsetRegister & 0xFC));
    commandPort.write(id);
    uint32_t result = dataPort.read();
    return result >> (8*(offsetRegister%4));
}

void PCI::Write(int16_t Bus, int16_t Device, int16_t Function, int32_t offsetRegister, int32_t msg){
    uint32_t id =   0x1<<31
                    | ((Bus & 0xFF) << 16)
                    | ((Device & 0x1F) << 11)
                    | ((Function & 0x07) << 8)
                    | ((offsetRegister & 0xFC));
    commandPort.write(id);
    dataPort.write(msg);
}


PCIDeviceDescriptor PCI::GetDeviceDescriptor(customOS::general::int16_t Bus, customOS::general::int16_t Device, customOS::general::int16_t Function){
    PCIDeviceDescriptor descriptor;

    descriptor.Bus = Bus;
    descriptor.Device = Device;
    descriptor.Function = Function;

    descriptor.Vendor_id = Read(Bus, Device, Function, 0x00);
    descriptor.Device_id = Read(Bus, Device, Function, 0x02);

    descriptor.Class_id = Read(Bus, Device, Function, 0x0B);
    descriptor.Subclass_id = Read(Bus, Device, Function, 0x0A);
    descriptor.Interface_id = Read(Bus, Device, Function, 0x09);

    descriptor.Revision = Read(Bus, Device, Function, 0x08);
    descriptor.Interrupt = Read(Bus, Device, Function, 0x3C);

    return descriptor;
}

void PCI::SelectDrivers(DriverManager * driverManager, InterruptManager* manager){
    for(int bus=0; bus<8; bus++){
        for(int device = 0; device < 32; device++){
            int nFun = 1;
            if(HasFunctions(bus, device))
                nFun = 8;
            else    
                nFun = 1;
            for(int function = 0; function<nFun; function++){
                PCIDeviceDescriptor descriptor = GetDeviceDescriptor(bus, device, function);

                if(descriptor.Vendor_id == 0x0000 || descriptor.Vendor_id == 0xFFFF)
                    continue;

                for(int barNum = 0; barNum < 6; barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        descriptor.Port = (uint32_t)bar.address;
                    
                    Driver* driver = GetDriver(descriptor, manager);
                    if(driver != 0)
                        driverManager->AddDriver(driver);
                }


                printf("PCI BUS ");
                printfHex(bus & 0xFF);
                
                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);
                
                printf(" = VENDOR ");
                printfHex((descriptor.Vendor_id & 0xFF00) >> 8);
                printfHex(descriptor.Vendor_id & 0xFF);
                printf(", DEVICE ");
                printfHex((descriptor.Device_id & 0xFF00) >> 8);
                printfHex(descriptor.Device_id & 0xFF);
                printf("\n");
            }
        }
    }
}






bool PCI::HasFunctions(int16_t Bus, int16_t Device){
     return Read(Bus, Device, 0, 0x0E) & (1<<7);
}


PCIDeviceDescriptor::PCIDeviceDescriptor(){

}

PCIDeviceDescriptor::~PCIDeviceDescriptor(){

}


Driver* PCI::GetDriver(PCIDeviceDescriptor dev, InterruptManager* interrupts){
    switch(dev.Vendor_id){
        case 0x1022: // AMD
            switch(dev.Device_id){
                case 0x2000: // am79c973
                    printf("AMD am79c973 ");
                    break;
            }
            break;

        case 0x8086: // Intel
            break;
    }
    
    
    switch(dev.Class_id){
        case 0x03: // graphics
            switch(dev.Subclass_id){
                case 0x00: // VGA
                    printf("VGA ");
                    break;
            }
            break;
    }
    
    
    return 0;
}



BaseAddressRegister PCI::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    BaseAddressRegister result;
    
    
    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;
    
    
    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;
    
    
    
    if(result.type == MemoryMapping){
        
        switch((bar_value >> 1) & 0x3){
            
            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }
        
    }
    else{   // InputOutput
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    
    
    return result;
}