#ifndef __CUSTOMOS_DRIVER_H
#define __CUSTOMOS_DRIVER_H

namespace customOS{
    namespace drivers{
        class Driver{
            public:
            Driver();
            ~Driver();
            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager{
            private:
            Driver *drivers[256];
            int numOfDrivers;

            public:
            DriverManager();
            ~DriverManager();
            void AddDriver(Driver *driver);
            void ActivateAll();


        };
    }
}

#endif
