#pragma once
#include "command_parser.h"
#include "ssd.h"
#include "buffer.h"

class DeviceController {
public:
    DeviceController(IDevice* device, Buffer* buffer_);
    int run(const CommandInfo& commandInfo);
private:
    IDevice* device_;
    Buffer* buffer_;
};