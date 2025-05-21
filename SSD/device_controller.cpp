#include "device_controller.h"

DeviceController::DeviceController(IDevice* device, Buffer* buffer) : device_(device), buffer_(buffer) { ; }

int DeviceController::run(const CommandInfo& commandInfo) {
	if (commandInfo.command == "R") {
		buffer_->readSSD(commandInfo.address);
	}
	else if (commandInfo.command == "W") {
		buffer_->updateBuffer(commandInfo.command, commandInfo.address, commandInfo.data);
	}
	else if (commandInfo.command == "E") {
		buffer_->updateBuffer(commandInfo.command, commandInfo.address, commandInfo.size);
	}
	else if (commandInfo.command == "F") {
		buffer_->flushBuffer();
	}
	else {
		cerr << "Unknown command: " << commandInfo.command << endl;
		return 1;
	}
    return 0;
}