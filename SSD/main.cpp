#include "gmock/gmock.h"
#include "ssd.h"

using namespace testing;
using namespace std;

bool isOutOf4ByteRange(const std::string& hexStr) {
    try {
        unsigned long long value = std::stoull(hexStr, nullptr, 16);
        return value > 0xFFFFFFFFULL;
    }
    catch (const std::exception&) {
        return true;
    }
}

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
    if (argc < 3) {
        cout << "Usage:" << endl;
        cout << "  ssd.exe R [address]" << endl;
        cout << "  ssd.exe W [address] [data(hex)]" << endl;
        return 1;
    }

    string cmd = argv[1];
    int address = stoi(argv[2]);

    if (address < 0 || address > 99)
    {
        cerr << "LBA Out of Range" << endl;
        return 1;
    }
    SSD ssd;
    
    cout << "CMD :" << cmd << endl;
    cout << "argc :" << argc << endl;
    if (cmd == "R") {
        ssd.read(address);
    }
    else if (cmd == "W") {
        if (argc < 4) {
            cerr << "ERROR" << endl;
            return 1;
        }
        if (isOutOf4ByteRange(argv[3])) {
            std::cout << "Out of 4-byte range!" << std::endl;
            return 1;
        }

        unsigned int data = std::stoul(argv[3], nullptr, 16);
        ssd.write(address, data);
        
    }
    else {
        cerr << "Unknown command: " << cmd << endl;
        return 1;
    }

	return 0;
#endif
}

