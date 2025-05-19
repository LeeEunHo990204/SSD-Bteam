#include "device_controller.h"

DeviceController::DeviceController(IDevice* device) : device_(device) { ; }

int DeviceController::run(const CommandInfo& commandInfo) {
    if (commandInfo.command == "W") {
        device_->write(commandInfo.address, commandInfo.data);
    }
    else if (commandInfo.command == "R") {
        unsigned int val = device_->read(commandInfo.address);
        std::cout << "Read: 0x" << std::hex << val << "\n";
    }
    else {
        std::cerr << "Invalid command\n";
        return 1;
    }
    return 0;
}