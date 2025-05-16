#include "gmock/gmock.h"

using namespace testing;
using namespace std;

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

    cout << "CMD :" << cmd << endl;
    cout << "argc :" << argc << endl;
    if (cmd == "R") {
        //readData(address);
        cout << "read~" << endl;
    }
    else if (cmd == "W") {
        if (argc < 4) {
            cerr << "ERROR" << endl;
            return 1;
        }
        cout << "write~" << endl;

        //writeData(address, data);
        
    }
    else {
        cerr << "Unknown command: " << cmd << endl;
        return 1;
    }
	return 0;
#endif
}

