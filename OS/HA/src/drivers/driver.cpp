#include <drivers/driver.h>
using namespace customOS::drivers;

Driver::Driver(){

}

Driver::~Driver(){

}
    
void Driver::Activate(){

}

int Driver::Reset(){

}

void Driver::Deactivate(){

}

DriverManager::DriverManager(){
    this->numOfDrivers = 0;
}


void DriverManager::AddDriver(Driver *driver){
    this->drivers[numOfDrivers] = driver;
    numOfDrivers++;
}
    
void DriverManager::ActivateAll(){
    for(int i = 0;i<numOfDrivers;i++)
        drivers[i]->Activate();
}