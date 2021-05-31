#include <hardwareCommunication/interrupts.h>
using namespace customOS::hardwareCommunication;
using namespace customOS::general;

void printf(char* str);

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp){
    if(ActiveInterruptManager!=0)
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    return esp;
}


uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp){
    

    
    if(handlers[interruptNumber]!=0){
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if(interruptNumber!=0x20){  //print non timmer interrupts
        char* msg = "Unhandled Interrupt 0x00";
        char* hex = "0123456789ABCDEF";
        msg[22] = hex[(interruptNumber>>4) & 0x0F];
        msg[23] = hex[(interruptNumber) & 0x0F];
        printf(msg);
    }

    if(0x20<=interruptNumber && interruptNumber <= 0x30){
        picMasterCommand.write(0x20);
        if(0x28<=interruptNumber)
            picSlaveCommand.write(0x20);
    }

    return esp;
}


InterruptManager* InterruptManager::ActiveInterruptManager = 0;

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
:picMasterCommand(0x20),
picMasterData(0x21),
picSlaveCommand(0xA0),
picSlaveData(0xA1)
{
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint16_t i = 0;i<256;i++){
        handlers[i] = 0;
        SetInterruptDescriptorTable(i, CodeSegment, &IgnoreInterrupt, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTable(0x20 + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTable(0x20 + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

        picMasterCommand.write(0x11);
        picSlaveCommand.write(0x11);

        picMasterData.write(0x20);
        picSlaveData.write(0x28);


        picMasterData.write(0x04);
        picSlaveData.write(0x02);

        picMasterData.write(0x01);
        picSlaveData.write(0x01);

        picMasterData.write(0x00);
        picSlaveData.write(0x00);



        InterruptDescriptorTablePointer idt;
        idt.size = 256*sizeof(GateDescriptor) - 1;
        idt.base = (uint32_t) interruptDescriptorTable;
        asm volatile("lidt %0 "::"m"(idt));
}


void InterruptManager::Activate(){
    if(ActiveInterruptManager!=0)
        ActiveInterruptManager->Deactivate();
    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate(){
    if(ActiveInterruptManager == this){
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

InterruptManager::~InterruptManager(){
    Deactivate();
}

void InterruptManager::SetInterruptDescriptorTable(
        uint8_t interruptnumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivilageLevel,
        uint8_t DescriptorType){
    
    const uint8_t IDT_DESC_PRESENT  = 0x80;

    interruptDescriptorTable[interruptnumber].handlerAddressLowBIts = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interruptnumber].handlerAddressHighBIts = (((uint32_t)handler)>>16) &0xFFFF;
    interruptDescriptorTable[interruptnumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptnumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilageLevel & 3) << 5);
    interruptDescriptorTable[interruptnumber].reserved = 0;

}














// InterruptHandler methods

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager){
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler(){
    if(interruptManager->handlers[interruptNumber]==this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp){
    return esp;
}