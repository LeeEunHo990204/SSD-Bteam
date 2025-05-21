#pragma once
#include "command_parser.h"
#include "ssd.h"
#include "buffer.h"

class BufferController {
public:
    BufferController(Buffer* buffer_);
    int run(const CommandInfo& commandInfo);
private:
    Buffer* buffer_;
};