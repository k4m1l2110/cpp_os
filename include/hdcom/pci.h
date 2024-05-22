//
// Created by user on 21.05.24.
//

#ifndef __CPPOS__HDCOM__PCI_H
#define __CPPOS__HDCOM__PCI_H

#import <hdcom/port.h>
#import <hdcom/interrupts.h>
#import <drivers/driver.h>
#import <common/types.h>


namespace cpp_os {
    namespace hdcom {
        //Peripheral Component Interconnect Device Descriptor
        class  PCID_descriptor {
        public:
            //Base Address Register
            common::uint32_t port_base;
            //Interrupt Request
            common::uint32_t interrupt;

            common::uint16_t bus;
            common::uint16_t device;
            common::uint16_t function;

            common::uint16_t vendor_id;
            common::uint16_t device_id;

            //Class Code
            common::uint8_t class_id;
            common::uint8_t subclass_id;
            common::uint8_t interface_id;

            common::uint8_t revision;

            PCID_descriptor();
            ~PCID_descriptor();
        };

        //Peripheral Component Interconnect Controller
        class PCIC {
            port32_t data_port;
            port32_t command_port;

        public:
            PCIC();
            ~PCIC();
            //Reads from the PCI Configuration Space
            common::uint32_t Read(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t register_offset);
            //Writes to the PCI Configuration Space
            void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t register_offset, common::uint32_t value);
            bool DeviceHasFunctions(common::uint16_t bus, common::uint16_t device);

            void SelectDrivers(drivers::DriverManager* driver_manager, cpp_os::hdcom::InterruptManager* interrupts);
            PCID_descriptor GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function);

        };
    }
}


#endif
