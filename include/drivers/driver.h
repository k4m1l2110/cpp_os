//
// Created by bonzo22137 on 5/19/24.
//

#ifndef __CPPOS__DRIVERS__DRIVER_H
#define __CPPOS__DRIVERS__DRIVER_H

namespace cpp_os {
    namespace drivers {

        class Driver {
        public:
            Driver() {}

            ~Driver() {}

            virtual void Activate();

            virtual int Reset();

            virtual void Deactivate();
        };

        class DriverManager {
            Driver *drivers[255];
            int numDrivers;
        public:
            DriverManager();

            void ActivateAll();

            void AddDriver(Driver *driver);

        };
    }
}

#endif
