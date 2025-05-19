#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "util.h"

struct CommandInfo {
    std::string command;
    unsigned int address;
    unsigned int data;
};

class CommandParser {
public:
    CommandParser(int argc, char* argv[]);  

    bool isValid() const;
    CommandInfo getCommandInfo() const;

private:
    bool valid = false;
    CommandInfo commandInfo;

    bool parse(int argc, char* argv[]);
};