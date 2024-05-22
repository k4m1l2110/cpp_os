//
// Created by user on 21.05.24.
//

#include <hdcom/pci.h>

cpp_os::hdcom::PCID_descriptor::PCID_descriptor() {
}

cpp_os::hdcom::PCID_descriptor::~PCID_descriptor() {
}

cpp_os::hdcom::PCIC::PCIC(): data_port(0xCFC), command_port(0xCF8){
    
}

cpp_os::hdcom::PCIC::~PCIC() {
}

cpp_os::common::uint32_t cpp_os::hdcom::PCIC::Read(cpp_os::common::uint16_t bus, cpp_os::common::uint16_t device, cpp_os::common::uint16_t function, cpp_os::common::uint32_t register_offset) {
    common::uint32_t id = 0x1 << 31;
    id |= (bus << 16);
    id |= (device << 11);
    id |= (function << 8);
    id |= (register_offset & 0xFC);
    command_port.Write(id);
    common::uint32_t result = data_port.Read();
    return result >> (8 * (register_offset % 4));
}

void cpp_os::hdcom::PCIC::Write(cpp_os::common::uint16_t bus, cpp_os::common::uint16_t device, cpp_os::common::uint16_t function, cpp_os::common::uint32_t register_offset, cpp_os::common::uint32_t value) {
    common::uint32_t id = 0x1 << 31;
    id |= (bus << 16);
    id |= (device << 11);
    id |= (function << 8);
    id |= (register_offset & 0xFC);
    command_port.Write(id);
    data_port.Write(value);
}

bool cpp_os::hdcom::PCIC::DeviceHasFunctions(cpp_os::common::uint16_t bus, cpp_os::common::uint16_t device) {
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void Print(char *str);
void PrintHex(cpp_os::common::uint8_t key);

void cpp_os::hdcom::PCIC::SelectDrivers(drivers::DriverManager* driver_manager, cpp_os::hdcom::InterruptManager* interrupts) {
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int num_functions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < num_functions; function++) {
                PCID_descriptor dev = GetDeviceDescriptor(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;
//                for (int bar_num = 0; bar_num < 6; bar_num++) {
//                    cpp_os::drivers::BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, bar_num);
//                    if (bar.address && (bar.type == drivers::InputOutput)) {
//                        dev.port_base = (common::uint32_t) bar.address;
//                    }
//                }
//                drivers::Driver* driver = driver_manager->GetDriver(dev, interrupts);
//                if (driver != 0) {
//                    driver_manager->AddDriver(driver);
//                }
                Print("PCI_BUS");
                PrintHex(bus & 0xFF);

                Print(" PCI_DEVICE");
                PrintHex(device & 0xFF);

                Print(" PCI_FUNCTION");
                PrintHex(function & 0xFF);

                Print(" PCI_VENDOR ID");
                PrintHex((dev.vendor_id >> 8) & 0xFF);
                PrintHex(dev.vendor_id & 0xFF);

                Print(" PCI_DEVICE ID");
                PrintHex((dev.device_id >> 8) & 0xFF);
                PrintHex(dev.device_id & 0xFF);
                Print("\n");
            }
        }
    }
}

cpp_os::hdcom::PCID_descriptor cpp_os::hdcom::PCIC::GetDeviceDescriptor(cpp_os::common::uint16_t bus, cpp_os::common::uint16_t device, cpp_os::common::uint16_t function) {
    PCID_descriptor result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);

    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}