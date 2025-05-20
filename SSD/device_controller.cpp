#include "device_controller.h"

DeviceController::DeviceController(IDevice* device) : device_(device) { ; }

int DeviceController::run(const CommandInfo& commandInfo) {
	if (commandInfo.command == "R") {
		device_->read(commandInfo.address);
	}
	else if (commandInfo.command == "W") {
		device_->write(commandInfo.address, commandInfo.data);
	}
	else if (commandInfo.command == "E") {
		device_->erase(commandInfo.address, commandInfo.size);
	}
	else {
		cerr << "Unknown command: " << commandInfo.command << endl;
		return 1;
	}
    return 0;
}