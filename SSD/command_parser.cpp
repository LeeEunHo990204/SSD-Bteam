#include "command_parser.h"


CommandParser::CommandParser(int argc, char* argv[]) {
    valid = parse(argc, argv);
}

bool CommandParser::parse(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  ssd.exe R [address]" << std::endl;
        std::cout << "  ssd.exe W [address] [data(hex)]" << std::endl;
        return false;
    }
    command = argv[1];
    address = std::stoi(argv[2]);

    if (argc == 4)
    {
        // [spec] lba range 에러 외에는 file에 error 적지 않음.
        if (isOutOf4ByteRange(argv[3])) {
            std::cout << "Out of 4-byte range!" << std::endl;
            return false;
        }
        data = std::stoul(argv[3], nullptr, 16);
    }
    return true;
       
}

bool CommandParser::isValid() const { return valid; }
std::string CommandParser::getCommand() const { return command; }
unsigned int CommandParser::getLBA() const { return address; }
unsigned int CommandParser::getData() const { return data; }