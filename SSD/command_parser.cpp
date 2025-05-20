#include "command_parser.h"


CommandParser::CommandParser(int argc, char* argv[]) {
    valid = parse(argc, argv);
}

bool CommandParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  ssd.exe R [address]" << std::endl;
        std::cout << "  ssd.exe W [address] [data(hex)]" << std::endl;
        std::cout << "  ssd.exe E [address] [size(dec)]" << std::endl;
        std::cout << "  ssd.exe F" << std::endl;
        return false;
    }
    commandInfo.command = argv[1];
    if (2 < argc) {
        commandInfo.address = std::stoi(argv[2]);
    }

    if (argc == 4)
    {
        // [spec] lba range 에러 외에는 file에 error 적지 않음.
        if (isOutOf4ByteRange(argv[3])) {
            std::cout << "Out of 4-byte range!" << std::endl;
            return false;
        }

        if (commandInfo.command == "W") {
            commandInfo.data = std::stoul(argv[3], nullptr, 16);
        }
        else{
            commandInfo.data = std::stoul(argv[3], nullptr, 10);
        }
    }
    return true;
       
}

bool CommandParser::isValid() const { return valid; }
CommandInfo CommandParser::getCommandInfo() const{ return commandInfo; }
