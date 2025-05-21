#include "buffer_controller.h"

BufferController::BufferController(Buffer* buffer) : buffer_(buffer) { ; }

int BufferController::run(const CommandInfo& commandInfo) {
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