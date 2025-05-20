#pragma once
#include "command_parser.h"
#include "ssd.h"


class DeviceController {
public:
    DeviceController(IDevice* device);
    int run(const CommandInfo& commandInfo);
private:
    IDevice* device_; 
};