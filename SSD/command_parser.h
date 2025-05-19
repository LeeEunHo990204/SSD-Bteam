#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "util.h"

class CommandParser {
public:
    CommandParser(int argc, char* argv[]);

    bool isValid() const;
    std::string getCommand() const;         // e.g. "W"
    unsigned int getLBA() const;         // e.g. 0x10
    unsigned int getData() const;        // e.g. 0xABCD1234

private:
    bool valid = false;
    std::string command;
    unsigned int address = 0;
    unsigned int data = 0;

    bool parse(int argc, char* argv[]);
};